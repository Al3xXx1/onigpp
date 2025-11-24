# Copilot 指示書

## 技術スタック

- C++11 and later

## リポジトリの概要

- `oniguruma/` - サブモジュール
- `src/` - ソース
- `tests/` - テスト群
- `tests/compat/` - `compat_test.cpp`がある場所
- `tests/compat/patterns.json` - `compat_test`でテストするパターン項目を書いたJSON
- `onigpp.h` - 主要ヘッダ。

## 指示

- コメントは英語でお願いします。
- JSONでは、`\xXX` 形式は使わず、`\uXXXX`を使用してください。
- `compat_test`でテスト項目が失敗したときは、`compat_test`の実行を失敗させてください。
- `compat_test`で`std::regex`側が失敗したときは、C++のバージョンを確認して、そのテスト項目がC++のバージョンに関わらず、常に失敗するときは、`compat_test`の`std::regex`側のテストを修正してください。C++のバージョンにより、失敗項目が変化するときは、C++のバージョンによって失敗項目を場合分けしてください。ただし、挙動はフラグ群の指定によります。
- `compat_test`で`onigpp`側が失敗したときは、`onigpp`側を修正してください。ただし、修正はフラグ群の指定によります。

## 短期目標

- https://github.com/katahiromz/onigpp/actions/runs/19634520414/job/56221911094 と https://github.com/katahiromz/onigpp/actions/runs/19634520414/job/56221911093 のエラーを修正してください。
