#pragma once

#include <string>
#include <vector>
#include <utility>

class Quiz
{
    private:

        /*
            {
                (
                    first: 
                    (
                        first: ID_DOMANDA,
                        second: DOMANDA
                    ),
                    second:
                    (
                        first: RISPOSTA_CORRETTA,
                        second:
                        {
                            (
                                first: ID_RISPOSTA,
                                second: POSSIBILE_RISPOSTA
                            ),
                            (
                                first: ID_RISPOSTA,
                                second: POSSIBILE_RISPOSTA
                            ),
                            (
                                first: ID_RISPOSTA,
                                second: POSSIBILE_RISPOSTA
                            )
                        }
                    )
                )
            }
        */
        std::vector<std::pair<
            std::pair<int, std::string>, 
            std::pair<std::string, std::vector<std::pair<int, std::string>>>>> QuestionsAndAnswers;

        int GetIndexOfChar(const std::string &, const char &, int) const;
        std::string GetLineBetweenIndexes(const std::string &, const int, const int) const;
        std::string ReadQuestionFromLine(const std::string &) const;
        std::string ReadAnswerFromLine(const std::string &) const;
        std::vector<std::pair<int, std::string>> ReadAllAnswerFromLine(const std::string &) const;
        void LoadQuestions();
        bool HasUsersGuessed(const int, const int);
        
        int UserPoints; 
        std::vector<std::pair<std::string, std::string>> WrongUserAnswers;

    public:

        Quiz();
        void StartQuiz();

};