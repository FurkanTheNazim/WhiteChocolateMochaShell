# Minishell Lexer / Tokenizer Test Scenarios

Bu dosya, Lexer modülünün "Input" -> "Expected Tokens" dönüşümlerini doğrulamak için hazırlanmıştır.

## 1. Temel Komutlar (Basic)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Basit Komut | `ls` | `[WORD: ls]` |
| Argümanlı | `ls -la` | `[WORD: ls]`, `[WORD: -la]` |
| Çoklu Argüman | `echo hello world` | `[WORD: echo]`, `[WORD: hello]`, `[WORD: world]` |

## 2. Boşluk Yönetimi (Whitespace)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Bol Boşluklu | `ls        -la` | `[WORD: ls]`, `[WORD: -la]` |
| Tab Karakteri | `echo\tmerhaba` | `[WORD: echo]`, `[WORD: merhaba]` |
| Baş/Son Boşluk | `   ls -la   ` | `[WORD: ls]`, `[WORD: -la]` |

## 3. Operatörler ve Bitişik Durumlar (Metachars)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Basit Pipe | `ls|wc` | `[WORD: ls]`, `[PIPE: |]`, `[WORD: wc]` |
| Input Redir | `cat<file` | `[WORD: cat]`, `[REDIR_IN: <]`, `[WORD: file]` |
| Output Redir | `echo h>out` | `[WORD: echo]`, `[WORD: h]`, `[REDIR_OUT: >]`, `[WORD: out]` |
| Append (Çift) | `ls>>file` | `[WORD: ls]`, `[REDIR_APPEND: >>]`, `[WORD: file]` |
| Heredoc | `cat<<EOF` | `[WORD: cat]`, `[REDIR_HEREDOC: <<]`, `[WORD: EOF]` |
| Karışık | `cat<Makefile|grep` | `[WORD: cat]`, `[REDIR_IN: <]`, `[WORD: Makefile]`, `[PIPE: |]`, `[WORD: grep]` |

## 4. Tırnak İşaretleri (Quotes)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Tek Tırnak | `echo 'hello world'` | `[WORD: echo]`, `[WORD: 'hello world']` |
| Çift Tırnak | `echo "hello world"` | `[WORD: echo]`, `[WORD: "hello world"]` |
| Pipe Tırnakta | `echo "|"` | `[WORD: echo]`, `[WORD: "|"]` |
| Boş String | `echo ""` | `[WORD: echo]`, `[WORD: ""]` |
| Nested | `echo "'test'"` | `[WORD: echo]`, `[WORD: "'test'"]` |

## 5. Zorlayıcı Durumlar (Edge Cases)
| Senaryo | Girdi (Input) | Beklenen Token Listesi | Not |
| :--- | :--- | :--- | :--- |
| Yapışık Quote | `echo "Hel"lo` | `[WORD: echo]`, `[WORD: "Hel"lo]` | Boşluksuz |
| Operator Karışık | `echo >>> out` | `[WORD: echo]`, `[APPEND: >>]`, `[OUT: >]`, `[WORD: out]` | Greedy match |
| Quote İçi Quote | `echo " '" ` | `[WORD: echo]`, `[WORD: " '"]` | Literal kabul edilir |

## 6. Hata Durumları (Errors)
Lexer bu durumlarda hata döndürmeli veya prompt'u tekrar göstermelidir:
* `echo "unclosed` (Kapanmamış çift tırnak)
* `echo 'unclosed` (Kapanmamış tek tırnak)
* `\` veya `;` (Bonus yapılmıyorsa desteklenmeyen karakterler)

## 📝 Minishell Lexer To-Do List

Bu liste, ham string'i (input) alıp tertemiz bir Token Linked List'e çevirene kadar yapman gerekenleri adım adım içerir.

---

### 🛠 Bölüm 1: Hazırlık ve Yapı (Setup)
> Temeli sağlam atmazsak bina çöker.

- [ ] **Token Enum Tanımla** → `lexer_parser.h`
  - `TOKEN_WORD` — Komut/argüman kelimeleri
  - `TOKEN_PIPE` — `|` karakteri
  - `TOKEN_REDIR_IN` — `<` karakteri
  - `TOKEN_REDIR_OUT` — `>` karakteri
  - `TOKEN_REDIR_APPEND` — `>>` karakterleri
  - `TOKEN_HEREDOC` — `<<` karakterleri

- [ ] **Token Struct Tasarla** → `lexer_parser.h`
  - `char *content` — Token'ın içeriği (örn: "ls", "|")
  - `e_token_type type` — Enum tipi
  - `struct s_token *next` — Sonraki token'a pointer

- [ ] **Quote State Enum Tanımla** → `lexer_parser.h`
  - `STATE_DEFAULT` — Normal durum
  - `STATE_SQUOTE` — Tek tırnak içinde
  - `STATE_DQUOTE` — Çift tırnak içinde

- [ ] **Main Struct Güncelle** → `minishell.h` (veya ana header)
  - `t_token *token_list` — Lexer sonucu linked list başı

---

### 🔍 Bölüm 2: Yardımcı Fonksiyonlar (Helpers)
> Ana döngüyü kirletmemek için bu askerleri önden yaz.

- [ ] **`int is_whitespace(char c)`** → `lexer_utils.c`
  - `c == ' '` veya `c == '\t'` ise `1` döndür
  - Aksi halde `0` döndür

- [ ] **`int is_metachar(char c)`** → `lexer_utils.c`
  - `c` şunlardan biri mi: `|`, `<`, `>`
  - Evet → `1`, Hayır → `0`

- [ ] **`int is_quote(char c)`** → `lexer_utils.c`
  - `c == '\''` veya `c == '"'` ise `1` döndür

- [ ] **`void update_quote_state(char c, int *state)`** → `lexer_utils.c`
  - **Kritik fonksiyon!**
  - Eğer `state == DEFAULT` ve `c == '\'` → `state = SQUOTE`
  - Eğer `state == DEFAULT` ve `c == '"'` → `state = DQUOTE`
  - Eğer `state == SQUOTE` ve `c == '\'` → `state = DEFAULT`
  - Eğer `state == DQUOTE` ve `c == '"'` → `state = DEFAULT`

---

### ⚙️ Bölüm 3: Ana Motor (The Core Logic)
> String'in üzerinde yürüyen o meşhur `while` döngüsü.

- [ ] **`t_token *lexer(char *input)`** → `lexer.c`
  - Ana fonksiyon, input alıp token listesi döndürür
  - `int i = 0` ile string üzerinde yürür
  - Her iterasyonda aşağıdaki adımları uygular:

- [ ] **Whitespace Atlayıcı**
  ```c
  while (input[i] && is_whitespace(input[i]))
      i++;
  ```

- [ ] **Operatör Yakalayıcı** (`handle_operator`)
  - `input[i]` bir metachar mı kontrol et
  - Çiftli operatör kontrolü: `>>` veya `<<` mı?
    - Evet → 2 karakter al, uygun token oluştur
    - Hayır → 1 karakter al, tekli token oluştur
  - `i`'yi ilerlet (1 veya 2)

- [ ] **Kelime Yakalayıcı** (`handle_word`)
  - `start = i` kaydet
  - İç döngü başlat:
    ```c
    while (input[i] && !is_word_end(input[i], state))
    {
        update_quote_state(input[i], &state);
        i++;
    }
    ```
  - `ft_substr(input, start, i - start)` ile kelimeyi al
  - `TOKEN_WORD` tipinde token oluştur

- [ ] **Tırnak Hata Kontrolü**
  - Döngü bittikten sonra `state != DEFAULT` ise:
    - Hata mesajı yazdır: `"minishell: syntax error: unclosed quote"`
    - Token listesini temizle, `NULL` döndür

---

### 🔗 Bölüm 4: Liste Yönetimi (Linked List Utils)
> Listeye ekleme/çıkarma işleri.

- [ ] **`t_token *token_new(char *content, int type)`** → `token_utils.c`
  - `malloc(sizeof(t_token))` ile yer ayır
  - `node->content = content` (veya `ft_strdup`)
  - `node->type = type`
  - `node->next = NULL`
  - `return node`

- [ ] **`void token_add_back(t_token **head, t_token *new)`** → `token_utils.c`
  - `if (*head == NULL)` → `*head = new`
  - Değilse → son düğümü bul, `last->next = new`

- [ ] **`void token_clear(t_token **head)`** → `token_utils.c`
  - ⚠️ **Çok Önemli** — Memory leak önleyici
  - Listeyi baştan sona gez
  - Her düğüm için: `free(node->content)`, `free(node)`
  - `*head = NULL`

---

### 🧪 Bölüm 5: Test ve Entegrasyon
> Yaptıklarımız çalışıyor mu?

- [ ] **`void print_tokens(t_token *head)`** → `debug.c` (geçici)
  ```c
  while (head)
  {
      printf("[%d: %s]\n", head->type, head->content);
      head = head->next;
  }
  ```

- [ ] **Input Testleri**
  - Bu dosyanın üstündeki tüm senaryoları tek tek dene
  - Özellikle Edge Cases bölümüne dikkat!

- [ ] **Valgrind Kontrolü**
  ```bash
  valgrind --leak-check=full ./minishell
  ```
  - `exit` veya `Ctrl+D` sonrası "still reachable" dışında leak **olmamalı**

---

### 📁 Önerilen Dosya Yapısı
```
srcs/
├── lexer/
│   ├── lexer.c           # Ana lexer fonksiyonu
│   ├── lexer_utils.c     # is_whitespace, is_metachar, vs.
│   ├── token_utils.c     # token_new, token_add_back, token_clear
│   └── handle_word.c     # Kelime yakalama lojiği
└── parser/
    └── ...               # Sonraki aşama
```