#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<ctype.h>
#include<string.h>

// SABIT TANIMLAR
#define MAX_BOOKS 500
#define MAX_USERS 100
#define MAX_BORROWED 3
#define BORROWED_DAYS 15
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// KITAP YAPISI
typedef struct
{
    int id;
    char title[40];
    char author[30];
    char category[30];
    int isAvailable; // 1 => Bosta, 0 => Oduncte
    time_t dueTime; // Zaman asimini kontrol (sadece oduncte)
} Book;

// User Yapisi
typedef struct
{
    int id;
    char username[40];
    char password[40];
    int isStudent; // 1: öğrenci, 0: personel
    int borrowedBooks[MAX_BORROWED];
    int numBorrowed; // odunc alinan kitap sayisi
} User;

// Global Degiskenler
Book books[MAX_BOOKS];
User users[MAX_USERS];
int numBooks = 0; // kitap sayisi
int numUsers = 0; // kullanici sayisi
User *currentUser = NULL; // baslangicta currentUserin baska bir seye isaret etmesin


// Menuler
void mainMenu();
void adminMenu();
void userMenu();

// Kullacini Islemleri
void registerUser();
int login();
void logout();
void listAllUsers();

// Dosya Islemleri
void saveBooksToFile();
void loadBooksFromFile();
void saveUsersToFile();
void loadUsersFromFile();


// Kitap Islemleri
void addBook();
void updateBook();
void deleteBook();
void searchBook();
void borrowBook();
void returnBook();
void listAllBooks();
void listAllAvailableBooks();
void listOverDueBooks();
void viewBorrowedBooks();

// Yardimci Fonksiyonlar
int generateBookId();
int generateUserId();
int findBookById(int id);
void printBook(Book book);
void printUser(User user);
int findUserByUsername(char *username);
void clearInputBuffer();
time_t calculateDueDate();
void clearTerminal();




// Ana Fonksiyon
int main()
{
    // Dosyalardan Verileri Oku
    loadBooksFromFile();
    loadUsersFromFile();

     // Admin Hesabini Ekle (Eger Yoksa)
     if (findUserByUsername(ADMIN_USERNAME) == -1)
     {
         User admin = {
             .id = generateUserId(),
             .isStudent = 0
         };
         strcpy(admin.username , ADMIN_USERNAME);
         strcpy(admin.password , ADMIN_PASSWORD);
         users[numUsers++] = admin;
         // Degisiklikleri Dosyaya Yaz
         saveUsersToFile();
     }

    
    // Ana Menuyu Goster
    mainMenu();

    return 0;
}

// Main Menu
void mainMenu()
{
    // Terminali Temizle
    clearTerminal();
    int choice;

    do
    {
        printf("\033[1;34m+----------------------+\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;33m      ANA MENU      \033[0m \033[1;34m|\033[0m\n");
        printf("\033[1;34m+----------------------+\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m1. 1.GIRIS YAP      \033[0m \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m2. 2.CIKIS          \033[0m \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;31m3. Seciminiz:       \033[0m \033[1;34m|\033[0m\n");
        printf("\033[1;34m+----------------------+\033[0m\n");
        scanf("%d", &choice); // secenegi al 
        clearInputBuffer(); // '\n' icin temizle

        switch (choice)
        {
        case 1: {
            int userType = login();

            // 1 ise admin 0 ise kullanici menusu acilir
            if (userType == 1)
            {
                adminMenu();
            }else if (userType == 0)
            {
                userMenu();
            }
            break;
        }
        case 2: {
            printf("Program Kapatiliyor...\n");
            break;
        }
        default:
            printf("Gecersiz Secim!\n");
        }

    } while (choice != 2);
    

}

// Admin Menu
void adminMenu()
{
    
    // Terminali Temizle
    clearTerminal();

    int choice;
    do
    {
        printf("\033[1;34m+--------------------------------------+\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;33m             ADMIN MENU              \033[0m \033[1;34m|\033[0m\n");
        printf("\033[1;34m+--------------------------------------+\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m1.\033[0m Yeni Kullanici Ekle                \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m2.\033[0m Kitap Ekle                         \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m3.\033[0m Kitap Guncelle                     \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m4.\033[0m Kitap Sil                          \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m5.\033[0m Kitap Ara                          \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m6.\033[0m Geciken Kitaplari Listele          \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;31m7.\033[0m Tum Kullanicilari Listele          \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;31m8.\033[0m Cikis Yap                          \033[1;34m|\033[0m\n");
        printf("\033[1;34m+--------------------------------------+\033[0m\n");
        printf("\033[1;36mSeciminiz:\033[0m ");

        scanf("%d" , &choice);
        clearInputBuffer(); // '\n' icin temizle

        switch (choice)
        {
        case 1: registerUser(); break;
        case 2: addBook(); break;
        case 3: updateBook(); break;
        case 4: deleteBook(); break;
        case 5: searchBook(); break;
        case 6: listOverDueBooks(); break;
        case 7: listAllUsers(); break;
        case 8: logout(); break;
        default: printf("Gecersiz Secim!\n");
    }
    } while (choice != 8 && currentUser != NULL);
    
}

// Kullanici Menu
void userMenu()
{
    // Terminali Temizle
    clearTerminal();

    int choice;

    do
    {
        printf("\033[1;34m+--------------------------------------+\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;33m           KULLANICI MENU           \033[0m \033[1;34m|\033[0m\n");
        printf("\033[1;34m+--------------------------------------+\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m1.\033[0m Kitap Ara                         \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m2.\033[0m Kitap Odunc Al                    \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m3.\033[0m Kitap Iade Et                     \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;32m4.\033[0m Odunc Aldigim Kitaplar            \033[1;34m|\033[0m\n");
        printf("\033[1;34m|\033[0m \033[1;31m5.\033[0m Cikis Yap                         \033[1;34m|\033[0m\n");
        printf("\033[1;34m+--------------------------------------+\033[0m\n");
        printf("\033[1;36mSeciminiz:\033[0m ");

        scanf("%d", &choice);
        clearInputBuffer(); // '\n' icin temizle

        switch (choice)
        {
        case 1: searchBook(); break;
        case 2: borrowBook(); break;
        case 3: returnBook(); break;
        case 4: viewBorrowedBooks(); break;
        case 5: logout(); break;
        default: printf("Gecersiz Secim!\n");
        }
    } while (choice != 5 && currentUser != NULL);
    
}




// Kullanici Kaydi
void registerUser()
{
    
    // Terminali Temizle
    clearTerminal();

    // Kullanici Sayisini Kontrol Et
    if(numUsers >= MAX_USERS){
        printf("Maksimum Kullanici Sayisina Ulasildi !\n");
        return;
    }

    User newUser;
    newUser.id = generateUserId();
    newUser.numBorrowed = 0;
    memset(newUser.borrowedBooks, 0, sizeof(newUser.borrowedBooks)); // Yeni kullanicinin odunc kitap arrayini 0 la doldurur

    //Kullanici adi al
    printf("Kullanici Adi: ");
    scanf("%49s", newUser.username);
    clearInputBuffer(); // '\n' icin temizle

    if(findUserByUsername(newUser.username) != -1) {
        printf("Bu kullanici adi zaten alinmis!\n");
        return;
    } 

    //Sifre Al
    printf("Sifre: ");
    scanf("%49s", newUser.password);
    clearInputBuffer(); // '\n' icin temizle

    // Kullanici Tipi Al
    printf("Kullanici Tipi (1: Ogrenci, 0: Personel): ");
    scanf("%d", &newUser.isStudent);

    users[numUsers++] = newUser; // Yeni Kullniciyi Diziye Ekle Kullanici Sayisini Artir
    saveUsersToFile();

    printf("Kullanici Basariyla Kaydedildi!\n");
}

// Login
int login()
{
    
    // Terminali Temizle
    clearTerminal();
    
    char username[50];
    char password[50];

    printf("Kullanici Adi: ");
    scanf("%49s", username);
    clearInputBuffer(); // '\n' icin temizle

    printf("Sifre: ");
    scanf("%49s", password);
    clearInputBuffer();


    // Admin Kontrolu
    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0)
    {
        currentUser = &users[findUserByUsername(ADMIN_USERNAME)];
        return 1; // Admin Girisi
    }


    int userIndex = findUserByUsername(username);

    if (userIndex == -1)
    {
        printf("Kullanici Bulunamadi");
        return -1;
    }

    if (strcmp(users[userIndex].password , password) != 0)
    {
        printf("Hatali Sifre!!!");
        return -1;
    }

    currentUser = &users[userIndex];
    return 0; // Normal Kullanici
}

// Logout
void logout()
{
    
    // Terminali Temizle
    clearTerminal();

    currentUser = NULL;
    printf("Cikis Yapildi!!!");
} 

// Tum Kullanicilari Listele
void listAllUsers()
{
    // Terminali Temizle
    clearTerminal();

    int found = 0;
    printf("Tum Kullanici Listesi\n");
    printf("------------------------------------\n");

    for (int i = 0; i < numUsers; i++)
    {
        printUser(users[i]);
        found = 1;
    }

    if(!found){
        printf("Kullanici Bulunmamaktadir!!!\n");
    }
}




// Kitaplari Dosyaya Kaydetme
void saveBooksToFile()
{
    FILE *file = fopen("library.dat", "wb"); // wb binary modda yazmak icin
    
    if(file == NULL) // dosyada acilamazsa error ver
    {
        printf("Kitaplar dosyaya kaydedilirken bir sorun olustu!");
        return;
    }

    fwrite(&numBooks, sizeof(int), 1 , file); // kitap sayisini kaydet
    fwrite(books, sizeof(Book), numBooks, file); // kitaplari kaydet

    fclose(file); // dosyayi kapat
}

// Dosyadan kitaplari oku
void loadBooksFromFile()
{
    FILE *file = fopen("library.dat", "rb");// binary modda okumak icin

    if(file == NULL)
    {
        return;// Dosya yoksa devam et
    }

    fread(&numBooks, sizeof(int), 1, file);// Kitap sayisini oku
    fread(books, sizeof(Book), numBooks, file);// Kitaplari oku

    fclose(file);// Dosyayi kapat
}

// Kullanicilari dosyaya kaydet
void saveUsersToFile()
{
    FILE *file = fopen("users.dat", "wb");// binary modda yazmak icin
    
    if(file == NULL)// Dosya acilamazsa error ver
    {
        printf("Kullanicilar kaydedilirken hata olustu!\n");
        return;
    }

    fwrite(&numUsers, sizeof(int), 1, file);// kullanici sayisini kaydet
    fwrite(users, sizeof(User), numUsers, file);// kullanicilari kaydet

    fclose(file);// Dosyayi kapat
}

// Dosyadan kullanicilari oku
void loadUsersFromFile()
{
    FILE *file = fopen("users.dat", "rb"); // binary modda okumak icin
    if(file == NULL)
    {
        return; // Dosya yoksa devam et
    }

    fread(&numUsers, sizeof(int), 1, file); // Kullanici sayisini oku
    fread(users, sizeof(User), numUsers, file); // Kullanicilari oku
    
    fclose(file); // Dosyayi kapat
}





// Yeni Kitap Ekleme
void addBook()
{
    
    // Terminali Temizle
    clearTerminal();

    if (numBooks >= MAX_BOOKS)
    {
        printf("Maksimum Kitap Sayisina Ulasildi!!!\n");
        return;
    }
    
    // Yeni Kitap Olustur 
    Book newBook;
    newBook.id = generateBookId(); // Yeni kitap id'si ata
    newBook.isAvailable = 1; // Kitap Bosta

    // Kitap Basligini Al
    printf("Kitap Basligi Giriniz: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title , "\n")] = '\0'; // Sondaki newline karakterini NULL yap

    // Kitap Yazarini Al
    printf("Yazari Giriniz: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author , "\n")] = '\0'; // Sondaki newline karakterini NULL yap

    // Kitap Kategorisi Al
    printf("Kategori Giriniz: ");
    fgets(newBook.category, sizeof(newBook.category), stdin);
    newBook.category[strcspn(newBook.author , "\n")] = '\0'; // Sondaki newline karakterini NULL yap

    books[numBooks++] = newBook; // Yeni Kitabi Diziye Ekle ve Kitap Sayisini Artir
    saveBooksToFile(); // Yeni Kitabi Dosyaya Yaz
}

// Kitap Guncelleme
void updateBook()
{
    
    // Terminali Temizle
    clearTerminal();

    int id;
    // Tum Kitaplari Yazdir
    listAllBooks();

    printf("Guncellenecek Kitap Id: ");
    scanf("%d", &id);
    clearInputBuffer(); // '\n' icin temizle

    // Kitapbi kontrol et
    int index = findBookById(id); // 
    if (index == -1)
    {
        printf("Kitap Bulunmadi!\n");
        return;
    }

    // Guncelleme Islemleri
    printf("Yeni Baslik (Onceki: %s): ", books[index].title);
    fgets(books[index].title , sizeof(books[index].title) , stdin); // Yeni basligi al ve ata
    books[index].title[strcspn(books[index].title , "\n")] == '\0'; // Sondaki newline karakterini NULL yap

    printf("Yeni Yazar (Onceki: %s): ", books[index].author);
    fgets(books[index].author , sizeof(books[index].author) , stdin); // Yeni yazari al ve ata
    books[index].author[strcspn(books[index].author , "\n")] == '\0'; // Sondaki newline karakterini NULL yap
    
    printf("Yeni Kategori (Onceki: %s): ", books[index].category);
    fgets(books[index].category , sizeof(books[index].category) , stdin); // Yeni kategoriyi al ve ata
    books[index].category[strcspn(books[index].category , "\n")] == '\0'; // Sondaki newline karakterini NULL yap

    saveBooksToFile(); // Guncel Halini Dosyaya Yaz
    printf("Kitap Basariyla Guncellendi!\n");
}

// Kitap Silme
void deleteBook()
{

    // Terminali Temizle
    clearTerminal();

    int id;
    // Tum Kitaplari Yazdir
    listAllBooks();
    
    printf("Silmek Istediginiz Kitap Id: ");
    scanf("%d", &id);
    clearInputBuffer(); // '\n' icin temizle

    // Kitap var mi kontrol et
    int index = findBookById(id);
    if(index == -1){
        printf("Kitap Bulunamadi!\n");
        return;
    }

    // Kitap oduncte mi kontrol et 0 => oduncete
    if (!books[index].isAvailable)
    {
        printf("Bu kitap oduncte oldugu icin silinemez!!\n");
        return;
    }

    // Kitabi sil
    books[index] = books[numBooks -1]; // index numarali kitabin uzerine son kitabi yaz
    numBooks--; // kitap sayisini azalt

    saveBooksToFile(); // Yeni halini dosyaya yaz
    printf("Kitap Basariyla Silindi!\n");
}

// Kitap Arama
void searchBook()
{
    
    // Terminali Temizle
    clearTerminal();

    char query[50];
    // Istegi al
    printf("Kitap Arama (Baslik/Yazar/Kategori): ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0'; // sondaki newline karakterini null yap

    printf("Arama Sonuclari:\n");
    printf("-------------------------\n");

    int found = 0;

    // Kitabi Ara
    for (int i = 0; i < numBooks; i++)
    {
        if (strstr(books[i].title, query) != NULL ||
            strstr(books[i].author, query) != NULL ||
            strstr(books[i].category, query) != NULL)
            {
                printf("Kitap Bulundu\n");
                printBook(books[i]);
                found = 1;
            }
        
    }

    // Kitap Yoksa
    if (!found)
    {
        printf("Kitap Bulunamadi!!\n");
    }
}

//Kitap Odunc Alma
void borrowBook()
{
    
    // Terminali Temizle
    clearTerminal();

    // Giris Yapilmis mi
    if(currentUser == NULL){
        printf("Once Giris Yapmalisiniz!!!\n");
        return;
    }

    // Odunc Alma Limitini Kontrol Et
    if (currentUser->numBorrowed >= MAX_BORROWED)
    {
        printf("Maksimum Odunc Alma Limitine Ulastiniz!! (%d kitap).\n", MAX_BORROWED);
        return;
    }
    
    // Odunc Alma
    int id;
    // Raflardaki Kitaplari Yazdir
    listAllAvailableBooks();

    printf("Odunc Alinacak Kitap Id: ");
    scanf("%d", &id);
    clearInputBuffer(); // '\n' icin temizle

    // Kitabi bul
    int bookIndex = findBookById(id);

    // Kitap var mi 
    if (bookIndex == -1)
    {
        printf("Kitap Bulunamadi!!\n");
    }

    // Kitap Oduncte mi
    if(!books[bookIndex].isAvailable){
        printf("Istediginiz Kitap Oduncte :( \n");
        return;
    }

    // Kitabi Odunc Ver
    books[bookIndex].isAvailable = 0;
    books[bookIndex].dueTime = calculateDueDate();

    // Kullanicinin Odunc Listesine Ekle, Kitap Sayisini Artir
    currentUser->borrowedBooks[currentUser->numBorrowed++] = id;
    

    // Degisiklikleri Dosyaya Kaydet
    saveBooksToFile();
    saveUsersToFile();

    char dueTimeStr[20];
    // Iade Tarihini Hesapla
    strftime(dueTimeStr , sizeof(dueTimeStr) , "%d.%m.%Y" , localtime(&books[bookIndex].dueTime));
    printf("Kitap Odunc Verildi. Iade Tarihi: %s\n", dueTimeStr);
}

// Kitap Iade Etme
void returnBook()
{
    // Terminali Temizle
    clearTerminal();

    // Giris Yapilmis mi
    if(currentUser == NULL){
        printf("Once Giris Yapmalisiniz!!!\n");
        return;
    }

    // Odunc Alidigi Kitap Var mi
    if (currentUser->numBorrowed == 0) {
        printf("Ödünç aldığınız kitap bulunmamaktadır.\n");
        return;
    }

    int id;
    printf("Iade Etmek Istediginiz Kitap ID: ");
    scanf("%d", &id);
    clearInputBuffer(); // '\n' icin temizle

    // Kitap Bu Kullanici Tarafindan Odunc Mu Alinmis Kontrol Et
    int borrowedIndex = -1;
    for (int i = 0; i < currentUser->numBorrowed; i++)
    {
        if (currentUser->borrowedBooks[i] == id)
        {
            borrowedIndex = i;
            break;
        }
    }

    if (borrowedIndex == -1)
    {
        printf("Bu kitabi Odunc Almamissiniz");
        return;
    }

    int bookIndex = findBookById(id);
    if (bookIndex == -1)
    {
        printf("Kitap Bulunamadi!!\n");
        return;
    }

    // Kitabi Iade Et
    books[bookIndex].isAvailable = 1;

    // Kullanicinin Odunc Listesinden Cikar
    for (int i = borrowedIndex; i < currentUser->numBorrowed -1; i++)
    {
        currentUser->borrowedBooks[i] = currentUser->borrowedBooks[i+1];
    }// iade edilen kitaptan sonraki kitaplari bir sola kaydir
    
    // Odunc Alinan Kitap Sayisini Azalt
    currentUser->numBorrowed--;

    // Degisiklikleri Dosyaya Kaydet
    saveBooksToFile();
    saveUsersToFile();
    

    // Gecikme Kontrolu
    time_t now = time(NULL);
    if (now > books[bookIndex].dueTime)
    {
        int daysOverDue = (now - books[bookIndex].dueTime) / (60 * 60 * 24);
        printf("Kitap Basariyla Iade Edildi. %d Gun Gecikmis!\n", daysOverDue);
    } else{
        printf("Kitap Basariyla Iade Edildi!\n");
    }
}

// Tum Kitaplari Listeleme
void listAllBooks()
{
    // Terminali Temizle
    clearTerminal();
    
    int found = 0;

    printf("Tum Kitaplar:\n");
    printf("-----------------------------\n");

    for (int  i = 0; i < numBooks; i++)
    {
        printBook(books[i]);
        found = 1;
    }
    if(!found){
        printf("Kitap Bulunmamaktadir!!!\n");
    }
    
}

// Tum Musait Kitaplari Listeleme
void listAllAvailableBooks()
{
    // Terminali Temizle
    clearTerminal();

    int found = 0;

    printf("Raflardaki (Mevcut) Kitaplar:\n");
    printf("-------------------------------------\n");

    for (int i = 0; i < numBooks; i++) {
        if (books[i].isAvailable == 1) {
            printBook(books[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Raflarda hic kitap bulunmamaktadir!\n");
    }
}

// Geciken Kitaplari Listeleme (admin)
void listOverDueBooks()
{

    // Terminali Temizle
    clearTerminal();

    time_t now = time(NULL);
    int found = 0;

    printf("Geciken Kitaplar:\n");
    printf("---------------------------------\n");

    for (int i = 0; i < numBooks; i++)
    {
        if (!books[i].isAvailable && now > books[i].dueTime)
        {
            int daysOverDue = (now - books[i].dueTime) / (60 * 60 * 24);
            printBook(books[i]);
            printf("Gecikme Suresi: %d gun\n", daysOverDue);
            printf("--------------------------------------\n");
            found = 1;
        }
    }

    if (!found)
    {
        printf("Geciken Kitap Bulunmamaktadir.\n");
    }   
}

// Odunc Alinan Kitaplari Listeleme (admin)
void viewBorrowedBooks()
{
    
    // Terminali Temizle
    clearTerminal();
    
    // Girisi Kontrol Et
    if(currentUser == NULL){
        printf("Once Giris Yapmalisiniz!!\n");
        return;
    }

    // Odunc Alinan Kitap Var Mi
    if (currentUser->numBorrowed == 0)
    {
        printf("Odunc Aldiginiz Kitap Bulunmamaktadir!!\n");
        return;
    }

    printf("Odunc Aldiginiz Kitaplar:\n");
    printf("-----------------------------------\n");

    time_t now = time(NULL);
    for (int i = 0; i < currentUser->numBorrowed; i++)
    {
        // Odunc Alinan Kitaplarin Indexini Bul
        int bookIndex = findBookById(currentUser->borrowedBooks[i]);

        if (bookIndex != -1)
        {
            // Kitaplari Bilgilerini Yazdir
            printBook(books[bookIndex]);

            // Geciken Kitap Varsa Uyar
            if (now > books[bookIndex].dueTime)
            {
                int daysOverDue = (now - books[bookIndex].dueTime) / (60 * 24 * 24);
                printf("UYARI: Bu Kitap %d Gun Gecikmis!!!\n", daysOverDue);
            }
            printf("--------------------------------\n");
        }
    }
}








// Giris Bufferini temizle (\n icin)
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Yeni Kitap Id'si Uret
int generateBookId()
{
    // once artirir sonra dondurur
    static int lastId = 1000; // her cagrildiginda sifirlanmaz
    return ++lastId;
}

// Yeni Kullanici Id'si Uret
int generateUserId()
{
    // once artirir sonra dondurur
    static int lastId = 100; // her cagrildiginda sifirlanmaz
    return ++lastId;
}

// Kullanici Adina Gore Kullanici Bul
int findUserByUsername(char *username)
{
    for (int i = 0; i < numUsers; i++)
    {
        if (strcmp(users[i].username , username) == 0)
        {
            // i'inci kullanici
            return i;
        }
    }

    return -1; // Kullanici Bulunamadi
    
}

// Id'ye Gore Kitap Bul
int findBookById(int id)
{
    for (int i = 0; i < numBooks; i++)
    {
        if(books[i].id == id)
        {
            // i'inci kitapi don
            return i;
        }
    }
    
    return -1; // Kitap Bulunamadi
}

// Kitap Bilgilerini Yazdir
void printBook(Book book)
{
    printf("ID: %d\n", book.id);
    printf("Baslik: %s\n", book.title);
    printf("Yazar: %s\n", book.author);
    printf("Kategori: %s\n", book.category);
    printf("Durum: %s\n", book.isAvailable ? "Raflarda" : "Oduncte");

    // Oduncte ise
    if (!book.isAvailable)
    {
        char dueDateStr[20];

        // Iade Tarihini Hesapla
        strftime(dueDateStr , sizeof(dueDateStr) , "%d.%m.%Y" , localtime(&book.dueTime));
        printf("Iade Tarihi: %s\n", dueDateStr);

        // Gecikme Varsa Gecikmeyi Hesapla
        time_t now = time(NULL);
        if (now > book.dueTime)
        {
            int daysOverDue = (now - book.dueTime) / (60 *60 * 24);
            printf("Gecikme: %d gun\n", daysOverDue);
        }
    }
    printf("-------------------------------------\n");
}

// Kullanici Bilgilerini Yazdir
void printUser(User user)
{
    printf("ID: %d\n", user.id);
    printf("Isim: %s\n", user.username);
    printf("Tip: %s\n", user.isStudent ? "Ogrenci" : "Personel");
    printf("Odunc Aldigi Kitap Sayisi: %d\n", user.numBorrowed);

    printf("----------------------------------\n");
}

// Iade Tarihini Hesapla
time_t calculateDueDate()
{
    time_t now = time(NULL);
    return now + (BORROWED_DAYS * 24 * 60 * 60); // 15 Gun Ekle
}

// Terminali Temizle
void clearTerminal()
{
    system("cls||clear");
}