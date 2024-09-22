#ifndef CCALCULATOR_H
#define CCALCULATOR_H

#include <QString>
#include <QMap>

class CCalculator
{
    enum EOperation
    {
        Comma,
        LeftBracket,
        RightBracket,
        Plus,
        Minus,
        Multiply,
        Divide,
        Exponentiation,
        Pow,
    };

public:
    CCalculator();

    ~CCalculator();

    QString createReversePolish(QString str);

    double math(QString reversePolish);

private:
    void initialization();
    int getPriority(QString operation);
    bool checkStr(QVector<QString> array);
    bool isOperation(QString word);

    QVector<QString> separateString(QString str);

    double mathOperayion(QString A, QString B, QString operation);

private:
    QMap<QString, EOperation> m_mapOperationsValues;
};

#endif // CCALCULATOR_H
