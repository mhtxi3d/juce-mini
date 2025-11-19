# 中文輸入法技術文檔

本文檔說明倉頡輸入法使用的核心技術，並與其他常見中文輸入法進行比較分析。

## 目錄

1. [倉頡輸入法技術架構](#倉頡輸入法技術架構)
2. [其他中文輸入法技術](#其他中文輸入法技術)
3. [技術比較與優缺點分析](#技術比較與優缺點分析)

---

## 倉頡輸入法技術架構

### 1. 字碼系統 (Character Code System)

倉頡輸入法採用**字根分解**的方式，將漢字拆解為基本部件。

#### 1.1 字根表 (Radical Table)

倉頡輸入法使用 24 個基本字根，對應鍵盤上的 A-Y 鍵（不含 Z）：

| 按鍵 | 字根 | 類別 |
|------|------|------|
| A | 日 | 哲理類 |
| B | 月 | 哲理類 |
| C | 金 | 哲理類 |
| D | 木 | 哲理類 |
| E | 水 | 哲理類 |
| F | 火 | 哲理類 |
| G | 土 | 哲理類 |
| H | 竹 | 筆畫類 |
| I | 戈 | 筆畫類 |
| J | 十 | 筆畫類 |
| K | 大 | 筆畫類 |
| L | 中 | 筆畫類 |
| M | 一 | 筆畫類 |
| N | 弓 | 筆畫類 |
| O | 人 | 人身類 |
| P | 心 | 人身類 |
| Q | 手 | 人身類 |
| R | 口 | 人身類 |
| S | 尸 | 人身類 |
| T | 廿 | 人身類 |
| U | 山 | 人身類 |
| V | 女 | 人身類 |
| W | 田 | 人身類 |
| X | 難 | 特殊類 |
| Y | 卜 | 筆畫類 |

#### 1.2 輔助字形 (Secondary Forms)

每個主要字根下有多個輔助字形，例如：
- 「日」(A) 包含：日、曰、目 等
- 「月」(B) 包含：月、冂、⺼ 等

### 2. 查表機制 (Lookup Table Mechanism)

#### 2.1 字碼對應表結構

```cpp
// 基本資料結構
struct CangjieEntry {
    std::string code;        // 倉頡碼，例如 "ABCD"
    char32_t character;      // Unicode 字元
    int frequency;           // 使用頻率
};

// 查表實作
class CangjieLookupTable {
private:
    // 主要查詢表：字碼 -> 字元列表
    std::unordered_map<std::string, std::vector<char32_t>> codeToChar;

    // 反向查詢表：字元 -> 字碼
    std::unordered_map<char32_t, std::string> charToCode;

public:
    std::vector<char32_t> lookup(const std::string& code);
    std::string reverselookup(char32_t character);
};
```

#### 2.2 查表演算法

**時間複雜度分析：**
- 完全匹配查詢：O(1) - 使用雜湊表
- 前綴匹配查詢：O(k) - 使用 Trie 樹，k 為碼長

```cpp
// Trie 樹結構用於前綴查詢
class TrieNode {
public:
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    std::vector<char32_t> characters;  // 此節點對應的字元
    bool isEndOfWord = false;
};

class CangjieTrie {
private:
    std::unique_ptr<TrieNode> root;

public:
    void insert(const std::string& code, char32_t character);
    std::vector<char32_t> search(const std::string& prefix);
};
```

### 3. 重碼處理 (Collision Handling)

當多個字元對應同一組字碼時，系統需要處理重碼：

#### 3.1 頻率排序
```cpp
// 依使用頻率排序候選字
std::vector<char32_t> sortByFrequency(
    const std::vector<char32_t>& candidates,
    const FrequencyTable& freqTable
) {
    std::vector<std::pair<char32_t, int>> ranked;
    for (auto ch : candidates) {
        ranked.emplace_back(ch, freqTable.getFrequency(ch));
    }
    std::sort(ranked.begin(), ranked.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });
    // 返回排序後的字元
    std::vector<char32_t> result;
    for (const auto& p : ranked) {
        result.push_back(p.first);
    }
    return result;
}
```

#### 3.2 使用者學習
```cpp
// 記錄使用者選字習慣
class UserPreference {
private:
    std::unordered_map<std::string, std::unordered_map<char32_t, int>> userStats;

public:
    void recordSelection(const std::string& code, char32_t selected);
    std::vector<char32_t> adjustRanking(
        const std::string& code,
        const std::vector<char32_t>& candidates
    );
};
```

### 4. 編碼規則 (Encoding Rules)

#### 4.1 取碼原則

1. **首尾取碼**：取字的首、尾字根
2. **依書寫順序**：按照正確的筆順取碼
3. **最多五碼**：每個字最多取五個字根

#### 4.2 字碼長度分配

| 字碼長度 | 取碼規則 |
|----------|----------|
| 1 碼 | 特殊字（難字） |
| 2 碼 | 首 + 尾 |
| 3 碼 | 首 + 次 + 尾 |
| 4 碼 | 首 + 次 + 三 + 尾 |
| 5 碼 | 首 + 次 + 三 + 次尾 + 尾 |

---

## 其他中文輸入法技術

### 1. 拼音輸入法

#### 技術架構

```cpp
// 拼音音節對應
struct PinyinEntry {
    std::string pinyin;      // 拼音，如 "zhong"
    std::string tone;        // 聲調，1-4 或輕聲
    std::vector<char32_t> characters;  // 對應字元
};

// 拼音查表
class PinyinLookup {
private:
    // 拼音 -> 字元映射
    std::unordered_map<std::string, std::vector<PinyinEntry>> table;

    // 詞組資料庫
    std::unordered_map<std::string, std::vector<std::string>> phrases;

public:
    std::vector<char32_t> lookupSyllable(const std::string& pinyin);
    std::vector<std::string> lookupPhrase(const std::string& pinyinSequence);
};
```

#### 智慧選字

```cpp
// 基於統計語言模型
class LanguageModel {
private:
    // N-gram 模型
    std::unordered_map<std::string, double> unigram;
    std::unordered_map<std::string, double> bigram;
    std::unordered_map<std::string, double> trigram;

public:
    double probability(const std::vector<std::string>& context,
                       const std::string& word);
    std::string bestCandidate(const std::string& pinyin,
                              const std::vector<std::string>& context);
};
```

### 2. 注音輸入法

#### 技術架構

```cpp
// 注音符號對應
struct ZhuyinEntry {
    std::string zhuyin;      // 注音符號
    std::vector<char32_t> characters;
};

// 注音組合規則
class ZhuyinCombiner {
private:
    std::set<std::string> initials;    // 聲母
    std::set<std::string> medials;     // 介音
    std::set<std::string> finals;      // 韻母
    std::set<std::string> tones;       // 聲調

public:
    bool isValidCombination(const std::string& zhuyin);
    std::string normalize(const std::string& input);
};
```

### 3. 五筆輸入法

#### 技術架構

```cpp
// 五筆字根
struct WubiRadical {
    char key;                // 鍵位
    int zone;                // 區位 (1-5)
    int position;            // 位置
    std::vector<std::string> radicals;  // 字根列表
};

// 五筆編碼
class WubiEncoder {
private:
    std::unordered_map<std::string, std::vector<char32_t>> codeTable;

public:
    // 識別碼計算
    std::string getIdentifier(char32_t character);
    std::vector<char32_t> lookup(const std::string& code);
};
```

### 4. 手寫輸入法

#### 技術架構

```cpp
// 筆跡點
struct StrokePoint {
    float x, y;
    float pressure;
    uint64_t timestamp;
};

// 手寫辨識
class HandwritingRecognizer {
private:
    // 深度學習模型
    NeuralNetwork model;

public:
    std::vector<std::pair<char32_t, float>> recognize(
        const std::vector<std::vector<StrokePoint>>& strokes
    );
};
```

### 5. 語音輸入法

#### 技術架構

```cpp
// 語音辨識
class SpeechRecognizer {
private:
    AcousticModel acousticModel;
    LanguageModel languageModel;

public:
    std::string recognize(const AudioBuffer& audio);
    std::vector<std::string> getAlternatives(const AudioBuffer& audio);
};
```

---

## 技術比較與優缺點分析

### 1. 倉頡輸入法

#### 優點

| 優點 | 說明 |
|------|------|
| **重碼率低** | 由於編碼空間大（24^5 種組合），重碼率約 1-3% |
| **無需選字** | 大多數情況下輸入碼即得字 |
| **輸入速度快** | 熟練後可達 100+ 字/分鐘 |
| **離線可用** | 不需要網路或大型語言模型 |
| **記憶體佔用小** | 查表資料約 2-5 MB |
| **跨平台一致** | 編碼規則固定，各平台相同 |

#### 缺點

| 缺點 | 說明 |
|------|------|
| **學習曲線陡峭** | 需要記憶字根和拆字規則 |
| **不直覺** | 無法從讀音推斷編碼 |
| **拆字歧義** | 某些字的拆法有爭議 |
| **不支援模糊輸入** | 必須精確輸入字碼 |

#### 技術複雜度

```
記憶體使用：★★☆☆☆ (低)
CPU 使用：★☆☆☆☆ (極低)
學習成本：★★★★★ (高)
實作難度：★★☆☆☆ (中低)
```

### 2. 拼音輸入法

#### 優點

| 優點 | 說明 |
|------|------|
| **易學易用** | 會普通話即可使用 |
| **容錯性高** | 支援模糊音、簡拼 |
| **智慧聯想** | 詞組輸入效率高 |
| **持續優化** | 雲端學習使用者習慣 |

#### 缺點

| 缺點 | 說明 |
|------|------|
| **重碼率極高** | 同音字多，需頻繁選字 |
| **依賴語言模型** | 需要大型詞庫和統計模型 |
| **方言使用者不利** | 需要標準普通話發音 |
| **隱私疑慮** | 雲端輸入法可能收集資料 |

#### 技術複雜度

```
記憶體使用：★★★★☆ (高)
CPU 使用：★★★☆☆ (中)
學習成本：★☆☆☆☆ (極低)
實作難度：★★★★☆ (高)
```

### 3. 注音輸入法

#### 優點

| 優點 | 說明 |
|------|------|
| **台灣使用者熟悉** | 義務教育必學 |
| **符合發音習慣** | 直覺輸入 |
| **鍵位固定** | 不需切換鍵盤佈局 |

#### 缺點

| 缺點 | 說明 |
|------|------|
| **重碼率高** | 與拼音類似的問題 |
| **需專用鍵盤** | 或需記憶對應位置 |
| **非台灣使用者陌生** | 學習門檻較高 |

#### 技術複雜度

```
記憶體使用：★★★☆☆ (中)
CPU 使用：★★☆☆☆ (低)
學習成本：★★☆☆☆ (中低)
實作難度：★★★☆☆ (中)
```

### 4. 五筆輸入法

#### 優點

| 優點 | 說明 |
|------|------|
| **重碼率低** | 與倉頡類似 |
| **輸入速度快** | 專業打字員首選 |
| **支援詞組** | 可一次輸入多字詞 |

#### 缺點

| 缺點 | 說明 |
|------|------|
| **學習困難** | 需記憶字根位置 |
| **版本差異** | 86 版、98 版規則不同 |
| **簡體字優化** | 對繁體字支援較弱 |

#### 技術複雜度

```
記憶體使用：★★☆☆☆ (低)
CPU 使用：★☆☆☆☆ (極低)
學習成本：★★★★★ (高)
實作難度：★★☆☆☆ (中低)
```

### 5. 手寫輸入法

#### 優點

| 優點 | 說明 |
|------|------|
| **零學習成本** | 會寫字即可使用 |
| **適合生僻字** | 不知讀音也能輸入 |
| **觸控裝置友善** | 適合平板和手機 |

#### 缺點

| 缺點 | 說明 |
|------|------|
| **速度慢** | 遠低於鍵盤輸入 |
| **辨識率問題** | 草書或字跡潦草時準確度下降 |
| **硬體需求** | 需要觸控螢幕或手寫板 |
| **計算量大** | 深度學習模型耗資源 |

#### 技術複雜度

```
記憶體使用：★★★★★ (極高)
CPU 使用：★★★★★ (極高)
學習成本：★☆☆☆☆ (極低)
實作難度：★★★★★ (極高)
```

### 6. 語音輸入法

#### 優點

| 優點 | 說明 |
|------|------|
| **解放雙手** | 可同時進行其他操作 |
| **適合長文** | 口述比打字快 |
| **無障礙友善** | 適合視障或行動不便者 |

#### 缺點

| 缺點 | 說明 |
|------|------|
| **環境限制** | 噪音環境準確度低 |
| **隱私問題** | 公共場合不適用 |
| **標點困難** | 需口述標點符號 |
| **同音字問題** | 仍需後續校對 |

#### 技術複雜度

```
記憶體使用：★★★★★ (極高)
CPU 使用：★★★★★ (極高)
學習成本：★☆☆☆☆ (極低)
實作難度：★★★★★ (極高)
```

---

## 綜合比較表

| 輸入法 | 學習難度 | 輸入速度 | 重碼率 | 資源需求 | 適用場景 |
|--------|----------|----------|--------|----------|----------|
| 倉頡 | ★★★★★ | ★★★★★ | ★☆☆☆☆ | ★☆☆☆☆ | 專業打字、嵌入式系統 |
| 拼音 | ★☆☆☆☆ | ★★★☆☆ | ★★★★★ | ★★★★☆ | 一般使用者、初學者 |
| 注音 | ★★☆☆☆ | ★★★☆☆ | ★★★★☆ | ★★★☆☆ | 台灣使用者 |
| 五筆 | ★★★★★ | ★★★★★ | ★☆☆☆☆ | ★☆☆☆☆ | 專業打字（簡體） |
| 手寫 | ★☆☆☆☆ | ★☆☆☆☆ | ★★☆☆☆ | ★★★★★ | 觸控裝置、生僻字 |
| 語音 | ★☆☆☆☆ | ★★★★☆ | ★★★☆☆ | ★★★★★ | 長文輸入、無障礙 |

---

## 實作建議

### 倉頡輸入法實作要點

1. **資料結構選擇**
   - 使用 Trie 樹支援前綴查詢
   - 使用雜湊表加速完全匹配

2. **記憶體優化**
   - 字碼表可使用緊湊格式儲存
   - 考慮使用記憶體映射檔案

3. **效能優化**
   - 查表操作應為 O(1) 或 O(k)
   - 避免不必要的字串複製

4. **使用者體驗**
   - 提供即時候選字顯示
   - 支援使用者自訂字頻

### 範例程式碼

```cpp
// 完整的倉頡輸入引擎框架
class CangjieEngine {
public:
    CangjieEngine() {
        loadRadicalTable();
        loadCodeTable();
        loadUserPreferences();
    }

    // 主要查詢介面
    std::vector<char32_t> lookup(const std::string& code) {
        auto candidates = codeTable.lookup(code);
        candidates = userPref.adjustRanking(code, candidates);
        return candidates;
    }

    // 記錄使用者選擇
    void recordSelection(const std::string& code, char32_t selected) {
        userPref.recordSelection(code, selected);
    }

private:
    CangjieTrie codeTable;
    UserPreference userPref;

    void loadRadicalTable();
    void loadCodeTable();
    void loadUserPreferences();
};
```

---

## 參考資料

- 朱邦復，《倉頡輸入法》
- Unicode Consortium, "Unicode Standard"
- 各輸入法官方技術文檔
