# 🐢 MINISHELL ULTIMATE SUBJECT & GUIDE (TR)
**Sürüm:** Birleştirilmiş (Subject v9.0 + Norm v4.1 + Evaluation Sheet)
**Referans:** Bash (Şüpheye düştüğünde, Bash ne yapıyorsa onu yap)

---

## ⚠️ I. KRİTİK KURALLAR (Hata = 0 Puan)

### 1. Global Variable (KIRMIZI ÇİZGİ)
* **Kural:** **SADECE 1 ADET** global variable kullanmana izin verilir.
* **Amaç:** Bu global variable **SADECE** alınan sinyal numarasını (signal number) tutmak için kullanılabilir (örn: `g_signal`).
* **Yasak:** Global variable içinde struct, data veya history tutmak **YASAKTIR**. Global scope'ta "Norm" tipi yapılar kullanılamaz.

### 2. Memory Management (Bellek Yönetimi)
* **Leaks:** Memory leak'e izin verilmez (`valgrind` ile kontrol edilecektir).
* **Readline Leaks:** `readline()` fonksiyonu kendi içinde leak yapabilir (suppression dosyası ile geçilebilir), ancak **senin yazdığın kod** leak yapmamalıdır.
* **Free:** Heap'te allocate edilen (malloclanan) her şey gerektiğinde `free` edilmelidir.
* **Crash:** Segmentation Fault, Bus Error, Double Free vb. durumlar projenin **0 (Sıfır)** almasına neden olur.

### 3. Norm v4.1 Özel Durumları
* **İzin Verilenler:** `while`.
* **Yasaklananlar:** `for`, `do...while`, `switch`, `case`, `goto`, ternary operatörler (`? :`), VLA'lar (Variable Length Arrays - örn: `char str[len]`).
* **İsimlendirme (Naming):**
    * Struct'lar: `s_variable`.
    * Typedef'ler: `t_variable`.
    * Global'ler: `g_variable`.
* **Yapı:** Fonksiyon başına maks 25 satır, 4 argüman, 5 değişken.

---

## 🛠 II. İZİN VERİLEN FONKSİYONLAR
* **Readline:** `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`
* **Process:** `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `execve`, `exit`
* **Signals:** `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`
* **File/Dir:** `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `opendir`, `readdir`, `closedir`, `access`
* **IO:** `open`, `read`, `write`, `close`, `dup`, `dup2`, `pipe`, `ioctl`
* **Utils:** `malloc`, `free`, `printf`, `strerror`, `perror`, `getenv`
* **Terminal:** `isatty`, `ttyname`, `ttyslot`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`
* **Libft:** Kullanılabilir.

---

## 🐚 III. MANDATORY FEATURES (Shell Çekirdeği)

### 1. Prompt & History
* Yeni bir komut beklerken bir **prompt** göster (örn: `minishell$`).
* Çalışan bir **history** sistemin olsun (Önceki komutlara dönmek için Yukarı/Aşağı ok tuşları).
* **Evo Check:** Boş komutları (empty commands), sadece boşluk (space) veya tab içeren girdileri dene. Shell **crash** vermemeli.

### 2. Parsing & Quotes (Katı Kurallar)
* **Double Quotes (`"`):**
    * `$` (Dolar işareti) yorumlanmalı (interpret edilmeli).
    * `\` (backslash) veya `;` karakterleri **yorumlanmamalıdır**.
    * Örnek: `echo "$USER"` -> `fatih` basmalı.
* **Single Quotes (`'`):**
    * **HİÇBİR ŞEYİ** yorumlama. İçindeki her şey literal string'dir.
    * Örnek: `echo '$USER'` -> `$USER` basmalı.
* **Unclosed Quotes (Kapatılmamış Tırnaklar):** Subject bunu handle etmeni istemiyor (Undefined behavior), ancak programın crash vermemeli.

### 3. Redirections (Yönlendirmeler)
* `<` (Input Redirection): Girdiyi dosyadan al.
* `>` (Output Redirection): Çıktıyı dosyaya yaz (dosya varsa üzerine yaz/overwrite).
* `<<` (Heredoc): Belirtilen **delimiter** (ayırıcı) satırı görülene kadar input oku. (History'i güncellemek zorunda değil).
* `>>` (Append): Çıktıyı dosyaya ekle (append mode).

### 4. Pipes (`|`)
* Sol taraftaki komutun `stdout` çıktısını, sağ taraftaki komutun `stdin` girdisine bağla.
* Çoklu pipe'ları yönetebilmeli: `cmd1 | cmd2 | cmd3 ...`
* **Evo Check:** `ls |`, `| ls` gibi garip pipe kullanımlarını test et (Bash referans alınmalı).

### 5. Environment Variables & Expansion
* **Variable Expansion:** `$` işaretinden sonra gelen karakterler değerlerine genişletilmeli (expand edilmeli).
* **Exit Status:** `$?` o anki en son çalıştırılan foreground pipeline'ın exit status'üne (çıkış kodu) expand edilmeli.
* **Evo Check:** Başarılı komutlardan sonra (0) ve hatalı komutlardan sonra (127, 1 vb.) `$?` değerini kontrol et.

### 6. Signals (Sinyaller: Ctrl-C, Ctrl-D, Ctrl-\\)
* **Amaç:** Bash gibi davranmak.
* **Senaryo A: Boş Prompt (Interactive Loop)**
    * `Ctrl-C`: Yeni bir satıra geç ve yeni bir prompt göster.
    * `Ctrl-D`: Shell'den çıkış yap (`exit`).
    * `Ctrl-\`: Hiçbir şey yapma.
* **Senaryo B: Blocking Command (örn: `cat` veya `grep` çalışırken)**
    * `Ctrl-C`: Process'i öldür, newline bas.
    * `Ctrl-D`: EOF (End of File) gönder, shell üzerinde özel bir şey yapma.
    * `Ctrl-\`: Process'ten çık (Quit). (Bash'teki Core dump davranışı).
* **Evo Check:** Sinyalden sonra prompt'un temiz geldiğinden ve buffer'ın temizlendiğinden emin ol.

---

## ⚙️ IV. BUILT-INS (Kendin Implement Etmelisin)

Aşağıdaki komutlar `execve` ile çağrılmamalı, doğrudan shell içinde implement edilmelidir:

| Komut | Gereksinimler & Evo Check |
| :--- | :--- |
| **echo** | `-n` opsiyonu zorunlu. Boşlukları ve tırnakları doğru yönetmeli. |
| **cd** | Relative ve Absolute path desteklemeli. `PWD` ve `OLDPWD` güncellenmeli. İzinleri kontrol et. |
| **pwd** | Working directory'i basmalı. Opsiyon yok. |
| **export** | Değişken export etmeli. Argümansız ise değişkenleri (sıralı) listelemeli. |
| **unset** | Environment variable silmeli. Sonucu `env` ile kontrol et. |
| **env** | Mevcut environment variable'ları basmalı. Opsiyon veya argüman gerekmez. |
| **exit** | Shell'den çıkmalı. Exit status almalı. Sayısal olmayan argümanları hata olarak işlemeli. |

---

## 🧪 V. EXECUTION & PATH RESOLUTION

* **Absolute/Relative Path:** `/` veya `./` ile başlayan executable'lar direkt çalıştırılmalı.
* **PATH Variable:** `ls`, `grep` gibi komutlar `$PATH` içindeki dizinlerde aranmalı.
* **Edge Cases (Sınır Durumlar):**
    * `unset PATH` yap -> `ls` yaz -> Hata vermeli (Command not found).
    * `unset PATH` yap -> `/bin/ls` yaz -> Çalışmalı.
    * PATH içinde çoklu `:` kullanımı (`/bin::/usr/bin`) test edilmeli.
* **Hatalar:** `command not found` (127) ile `permission denied` (126) ayrımını doğru yap.

---

## 🎁 VI. BONUS (Sadece Mandatory %100 ise)
* `&&` ve `||` operatörleri (Parantez önceliği ile).
* Wildcard `*` (Current working directory için).