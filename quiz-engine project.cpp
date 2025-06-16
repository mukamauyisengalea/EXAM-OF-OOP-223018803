#include <iostream>
#include <cstring>
using namespace std;

// Structure to represent a quiz question
struct Question {
    char prompt[100];
    char choices[4][40];
    int correctIdx;
};

// Abstract base class
class QuizUser {
public:
    // Pure virtual function for quiz attempt
    virtual int attemptQuiz(Question* questions, int count) = 0;
    virtual ~QuizUser() {}
};

// GuestUser: can only attempt 3 questions
class GuestUser : public QuizUser {
public:
    int attemptQuiz(Question* questions, int count) override {
        cout << "\n--- Guest User Quiz (Max 3 Questions) ---\n";
        int score = 0;
        int limit = (count < 3) ? count : 3;

        for (int i = 0; i < limit; i++) {
            Question* q = questions + i; // pointer arithmetic
            cout << "\nQ" << i + 1 << ": " << q->prompt << "\n";
            for (int j = 0; j < 4; j++) {
                cout << j + 1 << ". " << q->choices[j] << "\n";
            }

            int answer;
            cout << "Your answer (1-4): ";
            cin >> answer;

            if (answer - 1 == q->correctIdx) score++;
        }
        cout << "Guest Score: " << score << "/" << limit << "\n";
        return score;
    }
};

// PremiumUser: can attempt all questions
class PremiumUser : public QuizUser {
public:
    int attemptQuiz(Question* questions, int count) override {
        cout << "\n--- Premium User Quiz (All Questions) ---\n";
        int score = 0;

        for (int i = 0; i < count; i++) {
            Question* q = questions + i;
            cout << "\nQ" << i + 1 << ": " << q->prompt << "\n";
            for (int j = 0; j < 4; j++) {
                cout << j + 1 << ". " << q->choices[j] << "\n";
            }

            int answer;
            cout << "Your answer (1-4): ";
            cin >> answer;

            if (answer - 1 == q->correctIdx) score++;
        }
        cout << "Premium Score: " << score << "/" << count << "\n";
        return score;
    }
};

// Function to add a question dynamically
void addQuestion(Question*& questions, int& size, Question newQ) {
    Question* temp = new Question[size + 1];
    for (int i = 0; i < size; i++) {
        temp[i] = questions[i]; // copy old
    }
    temp[size] = newQ; // add new
    delete[] questions;
    questions = temp;
    size++;
}

// Function to remove a question at a specific index
void removeQuestion(Question*& questions, int& size, int index) {
    if (index < 0 || index >= size) return;
    Question* temp = new Question[size - 1];
    for (int i = 0, j = 0; i < size; i++) {
        if (i == index) continue;
        temp[j++] = questions[i];
    }
    delete[] questions;
    questions = temp;
    size--;
}

// Function to manually input a question
Question createQuestion(const char* prompt, const char* c1, const char* c2, const char* c3, const char* c4, int correct) {
    Question q;
    strncpy(q.prompt, prompt, sizeof(q.prompt));
    strncpy(q.choices[0], c1, sizeof(q.choices[0]));
    strncpy(q.choices[1], c2, sizeof(q.choices[1]));
    strncpy(q.choices[2], c3, sizeof(q.choices[2]));
    strncpy(q.choices[3], c4, sizeof(q.choices[3]));
    q.correctIdx = correct;
    return q;
}

// Main program
int main() {
    int qSize = 0;
    Question* questions = nullptr;

    // Add sample questions
    addQuestion(questions, qSize, createQuestion("Capital of Rwanda?", "Kigali", "Musanze", "Huye", "Rubavu", 0));
    addQuestion(questions, qSize, createQuestion("2 + 2 * 2 = ?", "6", "8", "4", "2", 0));
    addQuestion(questions, qSize, createQuestion("Sun rises in the?", "South", "North", "East", "West", 2));
    addQuestion(questions, qSize, createQuestion("Which is a programming language?", "HTML", "CSS", "Python", "Excel", 2));

    // Create dynamic user array
    int userCount = 2;
    QuizUser** users = new QuizUser*[userCount];
    users[0] = new GuestUser();
    users[1] = new PremiumUser();

    // Each user takes the quiz
    for (int i = 0; i < userCount; i++) {
        users[i]->attemptQuiz(questions, qSize);
    }

    // Clean up memory
    for (int i = 0; i < userCount; i++) {
        delete users[i];
    }
    delete[] users;
    delete[] questions;

    return 0;
}
