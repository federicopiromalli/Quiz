#include "../Include/Quiz.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>

Quiz::Quiz()
{
    this->UserPoints = 0;
    this->LoadQuestions();
}

/**
 * @brief Ottiene l'indice di un carattere saltandolo più volte se necessario.
 * @param Line La stringa da cui recuperare l'indice.
 * @param CharToFind Il carattere da cercare.
 * @param SkipCharForTimes Quante volte saltare il carattere.
 * @return L'indice trovato oppure -1 se non trovato.
 */
int Quiz::GetIndexOfChar(const std::string &Line, const char &CharToFind, int SkipCharForTimes) const
{
    for (int i = 0; i < Line.size(); ++i)
    {
        if (Line.at(i) == CharToFind && SkipCharForTimes > 0)
            --SkipCharForTimes;
        else if (Line.at(i) == CharToFind && SkipCharForTimes == 0)
            return i;
    }
    return -1;
}

/**
 * @brief Ottiene una porzione di stringa compresa tra due indici.
 * @param Line La stringa da cui recuperare la nuova sottostringa.
 * @param FirstIndex L'indice iniziale (escluso).
 * @param LastIndex L'indice finale (escluso).
 * @return La sottostringa compresa tra gli indici.
 */
std::string Quiz::GetLineBetweenIndexes(const std::string &Line, const int FirstIndex, const int LastIndex) const
{
    std::string NewLine;
    for (int i = FirstIndex + 1; i < LastIndex; ++i)
        NewLine.push_back(Line.at(i));
    return NewLine;
}

/**
 * @brief Estrae la domanda da una riga formattata.
 * @param Line La stringa da cui leggere la domanda.
 * @return La domanda estratta.
 */
std::string Quiz::ReadQuestionFromLine(const std::string &Line) const
{
    return this->GetLineBetweenIndexes(
        Line, this->GetIndexOfChar(Line, '(', 0), this->GetIndexOfChar(Line, ')', 0)
    );
}

/**
 * @brief Estrae la risposta corretta da una riga formattata.
 * @param Line La stringa da cui leggere la risposta corretta.
 * @return La risposta corretta.
 */
std::string Quiz::ReadAnswerFromLine(const std::string &Line) const
{
    const int SkipForTimes = std::stoi(this->GetLineBetweenIndexes(
        Line, this->GetIndexOfChar(Line, '[', 0), this->GetIndexOfChar(Line, ']', 0)
    ));
    return this->GetLineBetweenIndexes(
        Line, this->GetIndexOfChar(Line, '{', SkipForTimes), this->GetIndexOfChar(Line, '}', SkipForTimes)
    );
}

/**
 * @brief Estrae tutte le risposte possibili da una riga formattata.
 * @param Line La stringa da cui leggere le risposte.
 * @return Un vettore di coppie (ID, risposta) contenente le possibili risposte.
 */
std::vector<std::pair<int, std::string>> Quiz::ReadAllAnswerFromLine(const std::string &Line) const
{
    std::vector<std::pair<int, std::string>> Answers;
    for (int i = 0; i < 3; ++i)
    {
        Answers.push_back(std::make_pair(
            (i + 1), // Potrei usare solo i, ma per ragioni di leggibilita' aumento l'indice
            this->GetLineBetweenIndexes(
                Line, this->GetIndexOfChar(Line, '{', i), this->GetIndexOfChar(Line, '}', i)
            )
        ));
    }
    return Answers;
}

/**
 * @brief Carica tutte le domande e risposte dal file di testo.
 */
void Quiz::LoadQuestions()
{
    std::string CurrentLine;
    std::ifstream File("./Cache/Q&A.txt");

    int QuestionID = 0;
    while (std::getline(File, CurrentLine))
    {   
        this->QuestionsAndAnswers.push_back(
            std::make_pair(
                std::make_pair(++QuestionID, this->ReadQuestionFromLine(CurrentLine)),
                std::make_pair(
                    this->ReadAnswerFromLine(CurrentLine), 
                    this->ReadAllAnswerFromLine(CurrentLine)
                )
            )
        );
    }
}

/**
 * @brief Mostra tutte le domande e le relative risposte sul terminale.
 */
void Quiz::StartQuiz()
{
    int UserAnswerID;
    for (const auto &[QuestionPair, AnswerPair] : this->QuestionsAndAnswers)
    {  
        do
        {
            // Mostro la domanda
            std::system("cls");
            std::cout << "\n\n\tDomanda " << QuestionPair.first << ".\n";
            std::cout << "\t" << QuestionPair.second << "\n\n";
            
            // Mostro le risposte
            std::cout << "\tRisposte" << "\n";
            for (const auto &[AnswerID, Answer] : AnswerPair.second)
                std::cout << "\t" << AnswerID << ". " << Answer << "\n";

            // Chiedo la risposta
            std::cout << "\n\t=> Numero Risposta: ";
            std::cin >> UserAnswerID;
        }
        while 
        (UserAnswerID < 1 || UserAnswerID > AnswerPair.second.size());
        
        // Aumento i punti
        if (this->HasUsersGuessed(QuestionPair.first, UserAnswerID))
            ++this->UserPoints;
    }

    // Mostro il risultato e le soluzioni
    std::system("cls");
    std::cout << "\n\n\tRisultato: " 
        << this->UserPoints << " / " << this->QuestionsAndAnswers.size() 
        << "\n";
    std::cout << "\tRisposte Sbagliate e Soluzioni\n\n";

    for (const auto &[WrongAnswer, CorrectAnswer] : this->WrongUserAnswers)
    {
        std::cout << "\tX: "
                  << std::left << std::setw(15) << WrongAnswer
                  << "V: "
                  << std::left << std::setw(15) << CorrectAnswer
                  << "\n";
    }
}

/**
 * @brief Controlla se l'utente ha risposto correttamente.
 * @param QuestionId ID della domanda a cui si sta rispondendo.
 * @param UserAnswerID ID della risposta scelta dall'utente
 * @return Se l'utente ha risposto correttamente.
 */
bool Quiz::HasUsersGuessed(const int QuestionID, const int UserAnswerID)
{
    for (const auto &[QuestionPair, AnswerPair] : this->QuestionsAndAnswers)
    {
        if (QuestionPair.first == QuestionID)
        {
            for (const auto &[AnswerID, Answer] : AnswerPair.second)
            {
                if (AnswerID == UserAnswerID && AnswerPair.first == Answer)
                    return true;
                else if (AnswerID == UserAnswerID && AnswerPair.first != Answer)
                {
                    this->WrongUserAnswers.push_back(std::make_pair(Answer, AnswerPair.first));
                    return false;
                }
            }
        }
    }
    return false;
}
