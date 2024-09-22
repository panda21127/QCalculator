#include "Calculator.h"

#include <QVector>
#include <QStack>

#include <cmath>

CCalculator::CCalculator()
{
    initialization();
}

CCalculator::~CCalculator()
{
    m_mapOperationsValues.clear();
}

QString CCalculator::createReversePolish(QString str)
{
    QString result;
    QVector<QString> array = separateString("(" + str + ")");

    QStack<QString> stack;

    try
    {
        checkStr(array);
    }
    catch (const char* errorMessage)
    {
        throw errorMessage;
        return 0; //error
    }

    int i = 0;
    while (i < array.size())
    {
        if (array[i] == "-" && i != 0)
        {
            if (isOperation(array[i - 1]) && array[i - 1] != ")")
            {
                array[i + 1] = "!" + array[i + 1];
                array.remove(i);
                i--;
            }
        }
        else if (array[i] == "-")
        {
            array[i + 1] = "!" + array[i + 1];
            array.remove(i);
            i--;
        }

        i++;
    }

    for (QString word : array)
    {
        if (isOperation(word))
        {
            if (!stack.isEmpty())
            {
                if ((getPriority(word) <= getPriority(stack.back()))
                     && word != "("
                     && word != ")"
                     && word != ",")
                {
                    result = result + stack.pop() + " ";
                }
            }

            if (word != ")")
            {
                if (word != ",")
                {
                    stack.push(word);
                }
            }
            else
            {
                while (!stack.empty())
                {
                    QString operation = stack.pop();

                    if (operation == "(")
                    {
                        if (!stack.isEmpty() && stack.back() == "pow")
                        {
                            result = result + stack.pop() + " ";
                        }

                        break;
                    }

                    result = result + operation + " ";
                }
            }
        }
        else
        {
            result = result + word + " ";
        }
    }

    return result;
}

double CCalculator::math(QString reversePolish)
{
    QVector<QString> array = separateString(reversePolish);
    QStack<QString> stack;


    for (QString str : array)
    {
        stack.push(str);
    }

    while(stack.size() != 1)
    {
        if(stack.size() == 2)
        {
            throw "- _ -";
            return 0;
        }

        QStack<QString> tempStack;

        for (int i = 0; i < stack.size(); i++)
        {
            tempStack.push(stack[i]);

            if (isOperation(stack[i]))
            {
                double tempResult;

                try {
                    if ( i == 1)
                    {
                        throw "?_?";
                        return 0;
                    }

                    tempResult = mathOperayion(stack[i - 2], stack[i - 1], stack[i]);
                }
                catch (const char* errorMessage)
                {
                    throw errorMessage;
                    return 0; //error
                }

                tempStack.pop();
                tempStack.pop();
                tempStack.pop();
                tempStack.push(QString::number(tempResult));

                for (int j = i + 1; j < stack.size(); j++)
                {
                    tempStack.push(stack[j]);
                }

                stack = tempStack;

                break;
            }
        }
    }

    return stack[0].toDouble();
}

void CCalculator::initialization()
{
    m_mapOperationsValues[","] = Comma;
    m_mapOperationsValues["("] = LeftBracket;
    m_mapOperationsValues[")"] = RightBracket;
    m_mapOperationsValues["+"] = Plus;
    m_mapOperationsValues["-"] = Minus;
    m_mapOperationsValues["*"] = Multiply;
    m_mapOperationsValues["/"] = Divide;
    m_mapOperationsValues["^"] = Exponentiation;
    m_mapOperationsValues["pow"] = Pow;
}

int CCalculator::getPriority(QString operation)
{
    switch(m_mapOperationsValues[operation])
    {
        case Comma:
            return 0;
        case LeftBracket:
            return 1;
        case RightBracket:
            return 1;
        case Plus:
            return 2;
        case Minus:
            return 2;
        case Multiply:
            return 3;
        case Divide:
            return 3;
        case Exponentiation:
            return 4;
        case Pow:
            return 5;
    }

    return -1; // error
}

bool CCalculator::checkStr(QVector<QString> array)
{
    int countLeft = 0;
    int countRight = 0;

    for (QString word : array)
    {
        if (word == "(")
        {
            countLeft++;
        }

        if (word == ")")
        {
            countRight++;
        }
    }

    if (countLeft != countRight)
    {
        throw "Exeption";
        return false; // Error
    }

    for (QString word : array)
    {
        if (!isOperation(word))
        {
            try
            {
                bool ok;
                word.toDouble(&ok);

                if (!ok)
                {
                    throw "Exeption";
                }
            }
            catch (const char* errorMessage)
            {
                return false; //error
            }
        }
    }

    return true;
}


bool CCalculator::isOperation(QString word)
{
    for (QString operation : m_mapOperationsValues.keys())
    {
        if (word == operation)
        {
            return true;
        }
    }

    return false;
}

QVector<QString> CCalculator::separateString(QString str)
{
    QVector<QString> arrayArguments;
    QString tempStr = "";

    for (QChar word : str)
    {
        bool foundOperation = false;

        for (QString operation : m_mapOperationsValues.keys())
        {
            if (word == operation)
            {
                foundOperation = true;
            }
        }

        if (foundOperation)
        {
            if (tempStr.isEmpty())
            {
                arrayArguments.append(word);
            }
            else
            {
                arrayArguments.append(tempStr);
                arrayArguments.append(word);
            }

            tempStr.clear();
        }
        else
        {
            if (word != " ")
            {
                tempStr += word;

                if (m_mapOperationsValues.value(tempStr) == Pow)
                {
                    arrayArguments.append(tempStr);
                    tempStr.clear();
                }
            }
            else if (!tempStr.isEmpty())
            {
                arrayArguments.append(tempStr);
                tempStr.clear();
            }
        }
    }
    if (!tempStr.isEmpty())
    {
        arrayArguments.append(tempStr);
        tempStr.clear();
    }

    return arrayArguments;
}

double CCalculator::mathOperayion(QString A, QString B, QString operation)
{
    bool ok1;
    bool ok2;
    double number1 = 1;
    double number2 = 1;

    if (A[0] == "!")
    {
        number1 = -1;
    }

    if (B[0] == "!")
    {
        number2 = -1;
    }

    A.remove("!");
    B.remove("!");
    //A.remove(",");
    //B.remove(",");

    number1 *= A.toDouble(&ok1);
    number2 *= B.toDouble(&ok2);

    if (ok1 && ok2)
    {
        switch(m_mapOperationsValues[operation])
        {
        case Plus:
            return number1 + number2;
        case Minus:
            return number1 - number2;
        case Multiply:
            return number1 * number2;
        case Divide:
            return number1 / number2;
        case Exponentiation:
            return pow(number1, number2);
        case Pow:
            return pow(number1, number2);
        default:
            return 0;
        }
    }
    throw "Wrong number";
}
