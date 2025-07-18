# Change Log

## [Version 1.21.0](https://github.com/suzukiplan/vgszero/releases/tag/1.21.0)

NSF 再生機能を廃止して VGM 再生機能を追加しました。

サポートするチップチューン音源:

|音源名|概要|備考|
|:---:|:--|:--|
| NES APU | ファミコン音源 | 拡張音源は未サポート<br>DMC は CPU 依存機能を使用不可 |
| SN76489 (DCSG) | SG-1000, セガマークIII, ゲームギア等の音源 | ステレオ未サポート |
| AY-3-8910 (PSG) | MSX等の標準音源 | - |
| SCC | MSXのコナミ拡張音源 | SCC1 のみサポート (SCC2 は未サポート) |

VGM は version 1.61 以降の形式で出力しなければなりません。

使用例: example/15_vgm-asm

## [Version 1.20.0](https://github.com/suzukiplan/vgszero/releases/tag/1.20.0)

- ユーザ定義I/Oの範囲を 16 (0x00~0x0F) から 128 (0x00~0x7F) に拡張

## [Version 1.19.1](https://github.com/suzukiplan/vgszero/releases/tag/1.19.1)

- SDL2 版エミュレータで port 0x00 に書き込んだ値をログ出力する機能を追加
- SDCC 版ライブラリへ `vgs0_debug` を追加（ポート 0x00 へ文字列を書き込む）
- ユーザ定義I/Oの C 言語での利用例を追加: `example/18_debug`

## [Version 1.19.0](https://github.com/suzukiplan/vgszero/releases/tag/1.19.0)

### (User Definition I/O)

ポート番号 0x00 〜 0x0F は、ネイティブプログラムとの入出力に使用できるユーザ定義 I/O として利用できます。

例えば、Steam で UGC (リプレイデータなど) のダウンロード非同期リクエスト（out）をして結果のポーリング（in）をしたり、アチーブメントのアンロックをリクエストするなど、ネイティブコードで処理を実装しなければならない処理での利用を想定しています。

本機能を利用するには、`VGS0` クラス（エミュレータ）のインスタンスのメンバ変数 `userInCallback` または `userOutCallback` を設定する必要があります。

```c++
VGS0 vgs0;
vgs0.userInCallback = [](VGS0* vgs0, uint8_t port) -> uint8_t {
  return my_input_proc(port);
};

vgs0.userOutCallback = [](VGS0* vgs0, uint8_t port, uint8_t value) {
  my_output_proc(port, value);
};
```

## [Version 1.18.0](https://github.com/suzukiplan/vgszero/releases/tag/1.18.0)

### (拡張 RAM バンク I/O)

ポート番号 0xDC の入出力を行うことで、バンク切り替えをすることなく特定の拡張 RAM バンクの特定アドレスへの 1 byte の入出力を行うことができるようにしました。

```z80
; 入力
LD HL, 0x1FFF     ; 読み込みアドレスを HL に設定(※上位3ビットはマスクされる)
LD B, 123         ; 読み込むバンク番号を B に設定
IN A, (0xDC)      ; exram[123] の 0x1FFF 番地を A に読み込む
```

```z80
; 出力
LD HL, 0x1FFF     ; 書き込みアドレスを HL に設定(※上位3ビットはマスクされる)
LD B, 123         ; 書き込むバンク番号を B に設定
LD A, 0xCC        ; 書き込む値を A に設定
IN (0xDC), A      ; exram[123] の 0x1FFF 番地に A を書き込む
```

### (拡張 RAM のセーブ・ロード)

- ポート 0xDB の I/O で拡張 RAM バンクのセーブ（OUT）、ロード（IN）ができるようになりました
- セーブデータのファイル名は SD カードルートディレクトリ（SDL2の場合はカレントディレクトリ）の `saveXXX.dat` 固定です
  - `XXX` は拡張 RAM のバンク番号: `000` ~ `255`

セーブの実装例:

```z80
LD A, 123       # セーブする拡張 RAM バンク番号を A に設定
OUT (0xDB), A   # セーブを実行
AND 0xFF        # セーブ結果はレジスタAに格納される
JZ SAVE_SUCCESS # 成功時は 0
JNZ SAVE_FAILED # 失敗時は not 0
```

ロードの実装例:

```z80
LD A, 123       # ロードする拡張 RAM バンク番号を A に設定
IN A, (0xDB)    # ロード
JZ LOAD_SUCCESS # ロード成功時は 0
JNZ LOAD_FAILED # ロード失敗時は not 0 (※ロード先は 0x00 で埋められる)
```

- 1 つのセーブデータのサイズは 8192 bytes (バンクサイズ) 固定で領域の一部をセーブ・ロードすることはできません
- 通常のセーブデータと同様の注意事項があります

### (パーセンテージ計算のラップアラウンド)

0xD1 を OUT した結果が 32767 を超える場合は 32767、-32768 未満の場合は -32768 に丸められる仕様に変更しました。

### (パーセンテージ計算)

0xD1 を IN することで BC が DE の何パーセントになのか 0% 〜 255% の範囲で求めることができるようになりました。

```z80
LD BC, 33
LD DE, 100
IN A, (0xD1)  ; A = 33%
```

- DE が 0 の場合、結果は常に 0% になり、ゼロ除算エラーにはなりません
- 結果が 255% を超える場合は 255% に丸められます

### (16bit sin/cos)

0xD2, 0xD3 の OUT で HL に 16bit の sin/cos を求めることができるようになりました。

```z80
; 16bit sin
LD A, 123      ; A に求めるテーブル要素番号を指定
OUT (0xD2), A  ; HL = sin(A × π ÷ 128.0)

; 16bit cos
LD A, 123      ; A に求めるテーブル要素番号を指定
OUT (0xD3), A  ; HL = cos(A × π ÷ 128.0)
```

### (Add `E` command for vgs0)

SDL2 版エミュレータのデバッグコマンドに `E` (プロセス終了) を追加しました。

### (Add `NO_NSF` compile flag)

コンパイル時に `-DNO_NSF` を定義することで NSF を使用できない状態にするオプションを追加しました。。

### (Minor bugfix)

- macOS で tools のビルドが失敗する不具合を修正
- macOS で SDL2 版エミュレータのビルドが失敗する不具合を修正

## [Version 1.17.0](https://github.com/suzukiplan/vgszero/releases/tag/1.17.0)

フルアセンブリ言語で弾幕 STG の実装でよく用いられる 自機狙い (IN 0xD0), 自機ずらし (OUT 0xD0), 加速度計算 (OUT 0xD1) を簡単に実装できるようにするため、角度計算 (Angle Calculation) と 百分率計算 (Percentage Calculation) の HAGe を追加しました。

|   Port    |  I  |  O  | Description  |
| :-------: | :-: | :-: | :----------- |
|   0xD0    |  o  |  o  | 角度計算 |
|   0xD1    |  -  |  o  | 百分率計算 |

Examples の [12_angle-asm](./example/12_angle-asm/) の実装をこれらを用いた形に変更しています。

__(Angle Calculation)__

以下の 8 bytes の構造体が格納されたアドレスを HL に指定して 0xD0 を IN することで角度と16bit固定小数点の移動速度を計算することができます。

```c
struct rect {
    uint8_t x;      // X座標
    uint8_t y;      // Y座標
    uint8_t width;  // 幅
    uint8_t height; // 高さ
} chr[2];           // それらを 2 キャラクタ分（8bytes）
```

```z80
LD HL, 0xC000
IN A, (0xD0)
```

- `A` = 角度 (0〜255)
- `BC` = X 方向の移動速度（B = 整数, C = 少数）
- `DE` = Y 方向の移動速度（B = 整数, C = 少数）

また、 角度を指定して 0xD0 を OUT することで、その角度に対応する `BC` と `DE` を求めることができます。

まずは、0xD0 の IN で自機狙いの計算をして、求まった角度 A を加算したり減算して OUT をすることで自機ずらしを簡単に実装することができます。

__(Percentage Calculation)__

0xD1 を OUT することで、HLに格納された数値を 0% から 255% 範囲で計算することができます。

```
LD HL, 300
LD A, 150
OUT (0xD1), A   # HL = 450 (300 の 150%)
```

> HL の数値は符号付き 16bit 整数（signed short）として計算されます。

## [Version 1.16.0](https://github.com/suzukiplan/vgszero/releases/tag/1.16.0)

BG, FG, スプライトのインタレース描画機能を追加しました。

0x9F0C の設定により BG/FG の描画をインタレース（奇数行または奇数ピクセルの描画をスキップ）することができます。

__(0x9F0C)__

| Bit-7 | Bit-6 | Bit-5 | Bit-4 | Bit-3 | Bit-2 | Bit-1 | Bit-0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
|   -   |   -   |   -   |   -   | `FH`  | `FV`  | `BH`  | `BV`  |

- `FH` : FG の奇数スキャンラインの描画をスキップ（横インタレース）
- `FV` : FG の奇数ピクセルの描画をスキップ（縦インタレース）
- `BH` : BG の奇数スキャンラインの描画をスキップ（横インタレース）
- `BV` : BG の奇数ピクセルの描画をスキップ（縦インタレース）

スプライトは OAM の `attr2` の設定によりオブジェクト毎にインタレース描画を設定することができます。

__(Attribute2)__

| Bit-7 | Bit-6 | Bit-5 | Bit-4 | Bit-3 | Bit-2 | Bit-1 | Bit-0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
|   -   |   -   |   -   |   -   |   -   |   -   | `IH`  | `IV`  |

- `IH` : 奇数スキャンラインの描画をスキップ（横インタレース）
- `IV` : 奇数ピクセルの描画をスキップ（縦インタレース）

## [Version 1.15.1](https://github.com/suzukiplan/vgszero/releases/tag/1.15.1)

- [vgsasm](https://github.com/suzukiplan/vgsasm) を 1.2.2 から 1.2.4 に更新
  - Cannot specify nested structures with the `offset` operator.
  - `offset` calculation results for multi-count fields are not as expected.

## [Version 1.15.0](https://github.com/suzukiplan/vgszero/releases/tag/1.15.0)

- [vgsasm](https://github.com/suzukiplan/vgsasm) を version 1.0.0 から 1.2.2 に更新
  - `LD E, {IXH|IXL|IYH|IYL}` are incorrectly assembled to `LD C, {IXH|IXL|IYH|IYL}`.
  - Support `label+n` expression.
  - Support nested struct access.
- ROM to Memory DMA (OUT $C1) が想定通りに動作をしない不具合を修正
- マクロ `dma2mem` を vgszero.inc へ追加 

## [Version 1.14.0](https://github.com/suzukiplan/vgszero/releases/tag/1.14.0)

- ツールチェインに [vgsasm](https://github.com/suzukiplan/vgsasm) を追加
- 推奨アセンブラを z88dk から [vgsasm](https://github.com/suzukiplan/vgsasm) に変更
- 全ての example にアセンブリ言語版を追加

## [Version 1.13.0](https://github.com/suzukiplan/vgszero/releases/tag/1.13.0)

- [OAM16](./README.md#oam16) 機能を追加
- [example/17_clip](./example/17_clip/) を追加
- VGS0LIB に次の API を追加
  - `vgs0_oam_set16` ... OAM16 を用いたスプライト属性の一括設定マクロ
  - `VGS0_ADDR_OAM16` ... OAM16 の先頭アドレス
  - `OAM16` ... OAM16 構造体

## [Version 1.12.0](https://github.com/suzukiplan/vgszero/releases/tag/1.12.0)

Attribute の bit-4 を指定することで DPM のパターン番号を +1 する機能を追加
  
| Bit-7 | Bit-6 | Bit-5 | **Bit-4** | Bit-3 | Bit-2 | Bit-1 | Bit-0 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| `VI`  | `LR`  | `UD`  | **`PTN`** | `P3`  | `P2`  | `P1`  | `P0`  |

- 本機能は BG, FG, スプライトの全てで使用可能
- 本機能は DPM (Direct Pattern Mapping) を使用時に限り有効

## [Version 1.11.2](https://github.com/suzukiplan/vgszero/releases/tag/1.11.2)

- 未サポートの音楽データ形式を再生しようとするとクラッシュする不具合を修正
- RaspberryPi で NSF を再生し続けた時に `Assertion error` でクラッシュする場合がある不具合を修正
- NSF を再生した時にデフォルトトラック（NSF の先頭から 8 バイト目で指定されたトラック）を再生するように修正
- NSFPlayのコードリファクタ（不要コードの削除）
- example/03_sound を VGSBGM と NSF を混在させた形に変更
- README.md に推奨開発ツールの一覧を掲載
- README.md でマルチトラック NSF の使い方について詳述

## [Version 1.11.0](https://github.com/suzukiplan/vgszero/releases/tag/1.11.0)

- Extra RAM Bank の複製機能 `OUT ($B5)` を追加
- NSF 形式 のデータを用いた BGM (ファミコン標準音源 + VRC6) の再生をサポート（bgm.dat に .nsf を組み込みそれを指定して再生することで再生可能）
  - NSF 対応についての参考記事: https://note.com/suzukiplan/n/n94ea503ff2c8

## [Version 1.10.0 (2024.05.27)](https://github.com/suzukiplan/vgszero/releases/tag/1.10.0)

- [SDL2 版エミュレータ](./src/sdl2/) にデバッグオプション `-d` を追加

## [Version 1.9.0 (2024.05.26)](https://github.com/suzukiplan/vgszero/releases/tag/1.9.0)

- Extra RAM Bank (2MB RAM) を追加
- vgs0lib に上記機能を使用できる `vgs0_rambank_switch` 関数と `vgs0_rambank_get` 関数を追加

## [Version 1.8.0 (2024.05.24)](https://github.com/suzukiplan/vgszero/releases/tag/1.8.0)

- 特定の ROM バンクの内容を任意メモリに任意サイズだけ転送する DMA 機能（ROM to Memory DMA）を追加
- vgs0lib に上記機能を使用できる `vgs0_dma_ram` 関数を追加

## [Version 1.7.0 (2024.02.29)](https://github.com/suzukiplan/vgszero/releases/tag/1.7.0)

- BGM/SE のボリューム設定インタフェースを追加

## [Version 1.6.0 (2024.02.23)](https://github.com/suzukiplan/vgszero/releases/tag/1.6.0)

- [VGS-Zero SDK for Steam](https://github.com/suzukiplan/vgszero-steam) のリポジトリを分割

## [Version 1.5.0 (2024.02.19)](https://github.com/suzukiplan/vgszero/releases/tag/1.5.0)

- BG/FG に 1024 パターンモード を追加

## [Version 1.4.0 (2024.02.18)](https://github.com/suzukiplan/vgszero/releases/tag/1.4.0)

- VGS-Zero SDK for Steam を追加
- HAGe に収束型乱数（8bits, 16bits）を取得する機能を追加
- HAGe にパーリンノイズを取得する機能を追加
- vgs0lib に以下の関数を追加:
  - `vgs0_srand8` : 8-bits 乱数シードを設定
  - `vgs0_rand8` : 8-bits 乱数を取得
  - `vgs0_srand16` : 16-bits 乱数シードを設定
  - `vgs0_rand16` : 16-bits 乱数を取得
  - `vgs0_noise_seed` : パーリンノイズのシードを設定
  - `vgs0_noise_limitX` : パーリンノイズの X 座標縮尺を設定
  - `vgs0_noise_limitY` : パーリンノイズの Y 座標縮尺を設定
  - `vgs0_noise` : パーリンノイズを取得
  - `vgs0_noise_oct` : パーリンノイズを取得（オクターブあり）

## [Version 1.3.0 (2024.02.12)](https://github.com/suzukiplan/vgszero/releases/tag/1.3.0)

- vgs0lib に `vgs0_exit` 関数（割り込み禁止状態で HALT）を追加
- ラズパイ版で GPIO ジョイパッドによる入力に対応
- ラズパイ版で検証済みの USB ジョイパッドをドキュメント記載
- SDL2 版で `DI` している状態での `HALT` を検出した時にプロセスを停止するように変更
- SDL2 版でウィンドウの閉じるボタン（×）を押してもプロセスが終了しない不具合を修正
- SDL2 版でプログレッシブ表示（240x192）のキャプチャ保存に対応
- SDL2 版のウィンドウタイトル表示変更: VGS0 -> VGS-Zero

## [Version 1.2.3 (2024.01.30)](https://github.com/suzukiplan/vgszero/releases/tag/1.2.3)

- ラズパイで config.sys が正常に読み込まれない不具合を修正

## [Version 1.2.2 (2024.01.26)](https://github.com/suzukiplan/vgszero/releases/tag/1.2.2)

- ラズパイでリセット時に 16 フレームの空白フレームを挿入

## [Version 1.2.1 (2024.01.25)](https://github.com/suzukiplan/vgszero/releases/tag/1.2.1)

- ラズパイでBGMが再生されない問題を修正

## [Version 1.2.0 (2024.01.25)](https://github.com/suzukiplan/vgszero/releases/tag/1.2.0)

- LZ4 を削除

## [Version 1.1.0 (2024.01.23)](https://github.com/suzukiplan/vgszero/releases/tag/1.1.0)

- OAM の構造を変更
  - widthMinus1 と heightMinus1 を追加して 1x1 〜 16x16 のキャラクタパターンを 1 枚のスプライトとして表示できるようにする
  - bank を追加し OAM レコード毎に異なるキャラクタパターンを使用できるようにする
- OAM の構造変更に伴い [VRAM Memory Map を変更（※破壊的変更）](https://github.com/suzukiplan/vgszero/pull/14/files#diff-b335630551682c19a781afebcf4d07bf978fb1f8ac04c6bf87428ed5106870f5R325-R346)
- `vgs0_memset` が期待通りに動作しない不具合を修正
- 当たり判定のハードウェア機能を追加
- 乗算、除算、剰余残のハードウェア機能を追加
- 三角関数のハードウェア機能を追加
- ツールチェイン bmp2chr でパレットバイナリ出力に対応
- 画面表示をインタレース表示に変更
- SDCC 版 API に `vgs0_putstr` を追加
- ハードウェアリセット機能を追加（START+SELECT+A+B同時押しでリセット）
- SDL2 版エミュレータにデバッグ用機能を追加
  - R: リセット
  - S: キャプチャ（ram.bin, vram.bin, screen.bmp）

## [Version 1.0.0 (2024.01.01)](https://github.com/suzukiplan/vgszero/releases/tag/1.0.0)

初期リリース

