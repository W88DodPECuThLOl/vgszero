/**
 * Z80 Assembler for VGS-Zero
 * Copyright (c) 2024, Yoji Suzuki.
 * License under GPLv3: https://github.com/suzukiplan/vgsasm/blob/master/LICENSE.txt
 */
#pragma once
#include "common.h"

static bool isIncrementableRegister(LineData* line, std::vector<std::pair<TokenType, std::string>>::iterator it)
{
    if (it == line->token.end()) {
        return false;
    }
    if (it->first != TokenType::Operand) {
        return false;
    }
    switch (operandTable[it->second]) {
        case Operand::A: return true;
        case Operand::B: return true;
        case Operand::C: return true;
        case Operand::D: return true;
        case Operand::E: return true;
        case Operand::H: return true;
        case Operand::L: return true;
        case Operand::IXH: return true;
        case Operand::IXL: return true;
        case Operand::IYH: return false; // does not exist
        case Operand::IYL: return false; // does not exist
        case Operand::BC: return true;
        case Operand::DE: return true;
        case Operand::HL: return true;
        case Operand::IX: return true;
        case Operand::IY: return true;
    }
    return false;
}

static bool isIYHL(LineData* line, std::vector<std::pair<TokenType, std::string>>::iterator it)
{
    if (it == line->token.end()) {
        return false;
    }
    if (it->first != TokenType::Operand) {
        return false;
    }
    switch (operandTable[it->second]) {
        case Operand::IYH: return true;
        case Operand::IYL: return true;
    }
    return false;
}

void increment_split(std::vector<LineData*>* lines)
{
    bool retry;
    do {
        retry = false;
        for (auto it = lines->begin(); !retry && it != lines->end(); it++) {
            auto line = *it;
            auto prevToken = line->token.end();
            auto nextToken = line->token.end();
            for (auto token = line->token.begin(); token != line->token.end(); token++) {
                nextToken = token + 1;
                if (token->first == TokenType::Inc || token->first == TokenType::Dec) {
                    auto inc = token->first == TokenType::Inc;
                    auto pre = isIncrementableRegister(line, prevToken);
                    auto post = isIncrementableRegister(line, nextToken);
                    // 前後にレジスタが指定されてなければエラー
                    if (!pre && !post) {
                        pre = isIYHL(line, prevToken);
                        post = isIYHL(line, nextToken);
                        if (!pre && !post) {
                            line->error = true;
                            line->errmsg = "`++` or `--` can only be specified before or after the register.";
                        } else {
                            line->error = true;
                            line->errmsg = "`++` or `--` cannot be specified in the IYH or IYL registers.";
                        }
                        return;
                    }
                    // 前後両方がレジスタでもエラー
                    if (pre && post) {
                        line->error = true;
                        line->errmsg = "Illegal `++` or `--` sequence.";
                        return;
                    }
                    // トークンを後で削除するようにマーク
                    token->first = TokenType::Delete;
                    // 命令行を作成
                    auto newLine = new LineData(line);
                    newLine->token.clear();
                    newLine->token.push_back(std::make_pair(TokenType::Mnemonic, inc ? "INC" : "DEC"));
                    newLine->token.push_back(std::make_pair(TokenType::Operand, pre ? prevToken->second : nextToken->second));
                    // 命令行を挿入
                    if (pre) {
                        lines->insert(it + 1, newLine);
                    } else {
                        lines->insert(it, newLine);
                    }
                    // Deleteを差し引いた残トークンが 1 (レジスタのみ) かチェック
                    int tn = 0;
                    auto o = line->token.end();
                    for (auto w = line->token.begin(); w != line->token.end(); w++) {
                        tn += w->first != TokenType::Delete ? 1 : 0;
                        if (w->first == TokenType::Operand) {
                            o = w;
                        }
                    }
                    if (1 == tn && o != line->token.end()) {
                        o->first = TokenType::Delete;
                    }
                    // 最初から探索し直す
                    retry = true;
                    break;
                }
                prevToken = token;
            }
        }
    } while (retry);
}
