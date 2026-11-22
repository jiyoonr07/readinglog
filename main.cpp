#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// 부모 클래스
class Book {
private:
    string title;  // 제목
    string author; // 저자
    string readDate; // 읽은 날짜
    int rating; // 평점 (0 ~ 10)
    int pages; // 페이지 수
    string seriesName; // 시리즈 이름 (시리즈가 아니면 빈 글자)
    int seriesNum; // 시리즈 몇 번째 책인지 (시리즈가 아니면 0)

public: 
    // 생성자
    Book(string t, string a, string d, int r, int p, string sn, int snum) :
        title(t), author(a), readDate(d), rating(r), pages(p), 
        seriesName(sn), seriesNum(snum) { // 초기화 리스트

        // 평점 범위를 벗어나지 않도록 함
        if (rating < 0) rating = 0;
        if (rating > 10) rating = 10;
        // 페이지 수가 음수가 되지 않도록 함
        if (pages < 0) pages = 0;
    }

    // 소멸자
    virtual ~Book() {}

    // getter: 값 조회
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getReadDate() const { return readDate; }
    int getRating() const { return rating; }
    int getPages() const { return pages; }
    string getSeriesName() const { return seriesName; }
    int getSeriesNum() const { return seriesNum; }

    // 순수 가상 함수: 부모는 내용 없이 선언만 함
    // 자식 클래스가 반드시 overriding 해야 함
    virtual string getGenre() const = 0;
    virtual void printDetail() const = 0;
    virtual string getReview() const = 0; // 장르별 기록 내용 (서평 검색에 사용)

    // 연산자 오버로딩: 책 두 권을 평점으로 비교 (a > b 이면 a의 평점이 더 높음)
    bool operator>(const Book& other) const {
        return rating > other.rating;
    }

    // 모든 책이 가지고 있는 공통 정보 출력
    void printBasic() const {
        cout << "[" << getGenre() << "] " << title << " / " 
            << author << " / " << readDate << " / 평점: " << rating << "/10" 
            << " / " << pages << "쪽" << endl;
        if (seriesName != "") { // seriesName이 빈 string이 아닐때, 즉 시리즈일때만 출력
            cout << "   시리즈: " << seriesName << " " << seriesNum << "권" << endl;
        }
    }
};


// 자식 클래스 1: 문학
class Fiction : public Book {
private:
    string favQuote; // Fiction 클래스만이 가지는 정보: 인상깊은 구절
    string subGenre; // 세부 장르 (미스터리, 호러, 판타지 등), 입력하지 않으면 빈 글자(출력 안함)

public:
    // 생성자
    Fiction(string t, string a, string d, int r, int p, string sn, 
        int snum, string q, string sg = "") :
        Book(t, a, d, r, p, sn, snum), favQuote(q), subGenre(sg) {}

    // 함수 Overriding - 장르 출력
    string getGenre() const override {
        return "문학";
    }

    // 함수 Overriding - 책 전체 정보 출력
    void printDetail() const override {
        printBasic();
        if (subGenre != "") {   // 세부 장르를 입력했을 때만 출력
            cout << "   세부 장르: " << subGenre << endl;
        }
        cout << "   인상 깊은 구절: \"" << favQuote << "\"" << endl;
    }

    // 함수 Overriding - 서평 검색용 기록 내용
    string getReview() const override {
        return favQuote;
    }
};


// 자식 클래스 2: 비문학(Fiction 클래스와 거의 유사)
class NonFiction : public Book {
private:
    string learnedFact; // NonFiction 클래스만 가지는 정보: 배운 지식

public:
    NonFiction(string t, string a, string d, int r, int p, string sn, 
        int snum, string f) :
        Book(t, a, d, r, p, sn, snum), learnedFact(f) {}

    string getGenre() const override {
        return "비문학";
    }

    void printDetail() const override {
        printBasic();
        cout << "   배운지식: " << learnedFact << endl;
    }

    string getReview() const override {
        return learnedFact;
    }
};


// 자식 클래스 3: 시집
class Poetry : public Book {
private: 
    string favVerse; // Poetry 클래스만 가지는 정보: 와닿는 시구

public:
    Poetry(string t, string a, string d, int r, int p, string sn, 
        int snum, string v) :
        Book(t, a, d, r, p, sn, snum), favVerse(v) {}

    string getGenre() const override {
        return "시";
    }

    void printDetail() const override {
        printBasic();
        cout << "   와닿는 시구: \"" << favVerse << "\"" << endl;
    }

    string getReview() const override {
        return favVerse;
    }
};


// 서재 클래스: 책 목록을 가지고 관리
class Library {
private:
    vector<Book*> books; // 서재에 있는 모든 책 (Book 포인터 목록)

public:
    // 소멸자: 서재가 소멸될 때 책들도 함께 소멸되도록 함
    ~Library() {
        for (Book* b : books) {
            delete b;
        }
        books.clear();
    }

    // 메뉴 1. 책 추가 기능
    void addBook() {
        // 1) 장르 선택
        int genre;
        cout << "장르를 선택하세요 (번호 입력) (1. 문학  2. 비문학  3. 시): ";
        cin >> genre;
        cin.ignore(); // 입력 버퍼에 남은 \n 지우기

        if (genre < 1 || genre > 3) {
            cout << "잘못된 장르입니다. 메뉴로 돌아갑니다." << endl << endl;
            return;
        }

        // 2) 모든 책에 공통으로 존재하는 정보 입력
        string title, author, date;
        int rating, pages;
        cout << "제목: ";
        getline(cin, title); // 띄어쓰기도 인식되도록 getline 사용
        cout << "저자: ";
        getline(cin, author);
        cout << "읽은 날짜 (예: 2026-11-01): ";
        getline(cin, date);
        cout << "평점 (0-10): ";
        cin >> rating;
        cin.ignore();
        cout << "페이지 수: ";
        cin >> pages;
        cin.ignore(); // cin뒤에 getline 나오면 버퍼에 남은 \n 때문에 cin.ignore() 필요

        // 시리즈 정보 (선택)
        string seriesName;
        int seriesNum = 0;
        cout << "시리즈 이름 (시리즈가 아니면 엔터): ";
        getline(cin, seriesName);
        if (seriesName != "") { // 시리즈 이름이 입력되었을때만 몇번째 책인지 물어보도록 함
            cout << "시리즈 몇 번째 책인가요?: ";
            cin >> seriesNum;
            cin.ignore();
        }
        
        // 3) 장르별로 정보 입력 후 서재에 추가
        if (genre == 1) {
            string subGenre, quote;
            cout << "세부 장르 (없다면 엔터): ";
            getline(cin, subGenre);
            cout << "인상 깊은 구절: ";
            getline(cin, quote);
            books.push_back(new Fiction(title, author, date, rating, pages, 
                seriesName, seriesNum, quote, subGenre));
        }
        else if (genre == 2) {
            string fact;
            cout << "배운 지식: ";
            getline(cin, fact);
            books.push_back(new NonFiction(title, author, date, rating, pages, 
                seriesName, seriesNum, fact));
        }
        else {
            string verse;
            cout << "와닿는 시구: ";
            getline(cin, verse);
            books.push_back(new Poetry(title, author, date, rating, pages, 
                seriesName, seriesNum, verse));
        }

        cout << endl << "'" << title << "' 이(가) 서재에 추가되었습니다!" << endl << endl;
    }


    // 메뉴 2. 서재 열람 기능
    void viewAll() const {
        if (books.size() == 0) {
            cout << "아직 기록된 책이 없습니다." << endl << endl;
            return;
        }
        cout << "========== 내 서재 ==========" << endl << endl;
        for (int i = 0; i < (int)books.size(); i++) {
            cout << i + 1 << ". ";
            books[i]->printDetail();
            cout << endl;
        }
    }


    //메뉴 3. 독서 통계 열람 기능
    void showStats() const {
        if (books.size() == 0) {
            cout << "아직 기록된 책이 없습니다." << endl << endl;
            return;
        }

        // 1) 0에서 시작하는 변수를 만들어두고 셈
        int fictionCnt = 0, nonFictionCnt = 0, poetryCnt = 0; // 문학, 비문학, 시 책 개수
        int totalRating = 0; // 모든 평점의 합
        int totalPgs = 0; // 모든 페이지수의 합

        // 2) 책을 하나씩 세고, 해당하는 변수에 값 더하기
        for (Book* b : books) {
            if (b->getGenre() == "문학") fictionCnt++;
            else if (b->getGenre() == "비문학") nonFictionCnt++;
            else poetryCnt++;

            totalRating += b->getRating();
            totalPgs += b->getPages();
        }

        // 3) 평균 평점 계산
        double avgRating = (double)totalRating / books.size();

        // 4) operator> 사용해서 가장 평점이 높은 책 찾기
        Book* best = books[0];
        for (Book* b : books) {
            if (*b > *best) best = b;
        }

        // 5) 출력
        cout << "========== 독서 통계 ==========" << endl;
        cout << "총 읽은 책: " << books.size() << "권" << endl;
        cout << "  - 문학: " << fictionCnt << "권" << endl;
        cout << "  - 비문학: " << nonFictionCnt << "권" << endl;
        cout << "  - 시: " << poetryCnt << "권" << endl;
        cout << "평균 평점: " << fixed << setprecision(1) << avgRating << "/10" << endl;
        cout << "총 읽은 페이지: " << totalPgs << "쪽" << endl;
        cout << "가장 높은 평점: " << best->getTitle() << " (" << best->getRating() << "/10)" << endl << endl;
    }


    // 메뉴 4. 서평 검색 기능
    void search() const {
        if (books.size() == 0) {
            cout << "아직 기록된 책이 없습니다." << endl << endl;
            return;
        }

        // 1) 검색어 입력
        string key;
        cout << "검색어: ";
        cin.ignore(); // 메뉴 번호 입력 (cin >> num) 후 남은 \n 지우기
        getline(cin, key);

        if (key == "") {
            cout << "검색어를 입력하지 않았습니다." << endl << endl;
            return;
        }

        // 2) 모든 책을 하나씩 보면서 검색어가 들어 있는지 확인
        cout << endl << "========== '" << key << "' 검색 결과 ==========" << endl << endl;
        int found = 0; // 찾은 책 개수

        for (Book* b : books) {
            // 제목, 저자, 시리즈 이름, 기록 내용을 하나의 글자로 이어 붙임
            string text = b->getTitle() + " " + b->getAuthor() + " "
                + b->getSeriesName() + " " + b->getReview();

            if (text.find(key) != string::npos) { // 하나로 이어붙인 글자에 검색어가 들어 있다면
                b->printDetail(); // 찾은 책의 전체 정보 출력
                cout << endl;
                found++;
            }
        }

        // 3) 결과 개수 출력
        if (found == 0) cout << "검색 결과가 없습니다." << endl << endl;
        else cout << "총 " << found << "권을 찾았습니다." << endl << endl;

    }

    // 메인 메뉴 화면, 프로그램을 실행했을 때 가장 먼저 뜨는 화면
    void mainScrn() {
        int num;
        while (1) {
            cout << "========== 독서 기록장 ==========" << endl;
            cout << "1. 책 추가\n" << "2. 서재 열람\n" << "3. 독서 통계\n"
                << "4. 서평 검색\n" << "0. 종료\n" << endl;
            cout << "번호를 선택하세요: ";
            cin >> num;
            cout << endl;

            // 입력된 번호에 해당하는 메뉴 출력
            if (num == 1) addBook();
            else if (num == 2) viewAll();
            else if (num == 3) showStats();
            else if (num == 4) search();
            else if (num == 0) break;
            else cout << "잘못된 번호입니다. 다시 입력해 주세요." << endl << endl;
        }
        cout << "독서 기록장을 종료합니다." << endl;
    }
};


// main 함수
int main() {
    Library lib; // 서재 객체 lib 생성
    lib.mainScrn();

    return 0;
}