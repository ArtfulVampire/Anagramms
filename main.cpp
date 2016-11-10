#include <QtGui>
#include <QApplication>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cmath>
#include <QTextCodec>
#include <unistd.h>
#include <ios>
#include <fstream>
#include <algorithm>
#include <utility>
#include <chrono>
#include <random>
using namespace std;

std::vector<std::string> readWordsFile(const QString & filePath)
{
    std::vector<std::string> words;

    ifstream fil;
    fil.open(filePath.toStdString());
    std::string tmp;
    while(!fil.eof())
    {
        fil >> tmp;
        words.push_back(tmp);
    }
    fil.close();
    return words;
}


void sortFile(const QString & filePath) /// by alphabet
{
    std::vector<std::string> words = readWordsFile(filePath);

    std::sort(std::begin(words),
              std::end(words),
              [](const std::string & str1,
              const std::string & str2)
    {

        for(auto k = 0; k < std::min(str1.size(), str2.size()); ++k)
        {
            if(str1[k] > str2[k]) return false;
            else if(str1[k] < str2[k]) return true;
        };
    }
    );

    ofstream feel;
    feel.open(filePath.toStdString());

    for(auto str : words)
    {
        feel << str << endl;
    }
    feel.close();
}

ostream & operator << (ostream &os, QString toOut)
{
    os << toOut.toStdString();
    return os;
}

QString slash()
{
    return QString(QDir::separator());
}

QString str(const unsigned int input, int N = 3) // prepend zeros
{
    QString h;
    h.setNum(input);
    h.prepend(QString(N, '0'));
    return h.right(N);
}

QString SavePath(QDir * dir, int &i)
{
    QString h = dir->absolutePath();
    h += slash() + "anagramm" + str(i) + ".jpg";
    return h;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    srand(time(NULL));
	int num;
    std::vector<int> mixNum(8);

    int NumberOfFrames = 25;
    double FontSpace = 120.;
    double offsetX, offsetY, Radius, rad;
    double alpha;
    int letterNumber;
    int num1, num2;

    bool videoFlag;
    videoFlag = 0;
//    videoFlag = 1;

    ofstream outF;
    outF.open("/home/michael/Qt/Projects/AnagrammAnim/6letOut.txt");



    QString helpStr;
    QString helpString;
    QString HELP;
    QPixmap pic(1280, 1024);
    QPainter * pnt = new QPainter;
    QDir * dir = new QDir;
	pic.fill(QColor("gray"));
    pnt->begin(&pic);

    dir->cd("/media/Files"); //Michael

    dir->mkdir("Pictures");
    dir->cd("Pictures");
    dir->mkdir("Anagramms");
    dir->cd("Anagramms");
    dir->mkdir("FirstFrames");
    dir->mkdir("LastFrames");


	QFont myFont = QFont("Courier", FontSpace, QFont::Normal);

	pnt->setFont(myFont);
    pnt->setPen("lightgray");

	int q = 0;
	int AnagrammNumber = 1;
	int k = 1000;

    std::vector<std::string> wordList
            = readWordsFile("/home/michael/Qt/Projects/AnagrammAnim/6letNew.txt");
	cout << wordList.size() << endl;
	/// shuffle anagramms
	std::shuffle(std::begin(wordList),
				 std::end(wordList),
				 std::default_random_engine(
					 std::chrono::system_clock::now().time_since_epoch().count()));


	ofstream answers("/media/Files/Pictures/Anagramms/answers.txt");
    for(const std::string & word : wordList)
    {
		helpString = QString(word.c_str()); /// ow
        letterNumber = helpString.length();
        mixNum.resize(letterNumber);

        helpStr = helpString;
        HELP = helpString;

		int wordWidth = QFontMetrics(myFont).width(helpString);
		int letterHeight = QFontMetrics(myFont).xHeight();
		offsetX = pic.width() / 2 - wordWidth / 2;
		offsetY = pic.height() / 2.0 + letterHeight / 2;

        //begin of mixing
        std::iota(std::begin(mixNum),
                  std::end(mixNum),
                  0);

        //first mix of letters, beside first & last - second and pre-last?
        std::shuffle(std::begin(mixNum) + 1,
                     std::end(mixNum) - 1,
                     std::default_random_engine(
                         std::chrono::system_clock::now().time_since_epoch().count()));

        //now first
        do
        {
            num = rand() % (letterNumber - 2) + 1;
            std::swap(mixNum[num], mixNum[0]);
        } while (helpString[mixNum[0]] == helpString[mixNum[num]]);

        // and last
        do
        {
            num = rand() % (letterNumber - 1);
            std::swap(mixNum[num], mixNum[letterNumber-1]);
        } while (helpString[mixNum[letterNumber-1]] == helpString[mixNum[num]]);



        helpString.clear(); //mixed
        helpStr.clear(); //mixed
        for(int i = 0; i < letterNumber; ++i)
        {
            helpString += HELP[mixNum[i]];
        }
        helpStr = helpString;

        cout << HELP << "\t"
             << helpString << "\t"
             << helpStr << endl; //check mix
		answers << HELP.toStdString() << endl;




		pic.fill(QColor("black"));
        //draw the first frame
        pnt->drawText(offsetX, offsetY, helpString.toStdString().c_str());

        helpString = dir->absolutePath()
                     + slash() + "FirstFrames"
                     + slash() + "anagramm_"
                     + str(AnagrammNumber, 3) + ".jpg";
        pic.save(helpString, 0, 100);

        ++AnagrammNumber; continue; // only FirstFrames













        k=0; //number of current jpg-frame
        for(q=0; q<letterNumber/2+1; ++q) //begin N iterations of back-mixing
        {
            //first, second,...
            for(num2=0; num2<letterNumber; ++num2) //search the first(second,...) letter
            {
                if(mixNum[num2]==q) break;
            }

            //a @skeleton@
            helpString=""; //skeleton string
            helpStr=""; //what we have before letter changing
            for(int i=0; i<letterNumber; ++i)
            {
                helpStr.append(HELP[mixNum[i]]);
                if( (i!=q) && (i!=num2) )
                {
                    helpString.append(HELP[mixNum[i]]);
                }
                else
                {
                    helpString.append(" ");
                }
            }
            //cout<<helpStr<<endl;
            if(helpStr==HELP) break; //if all is on its place

            //draw all the frames into SavePath directory
            if(videoFlag == 1)
            {
                for(int i=0; i<NumberOfFrames+1; ++i)
                {
                    if(HELP[mixNum[q]] == HELP[mixNum[num2]]) break; //identycal letters exception

                    pic.fill(QColor("black"));

                    Radius=FontSpace*double(q+num2)/2.; //center of rotation
                    rad   =FontSpace*double(-q+num2)/2.; //radius of rotation -1 is an experimental bias 8-)
                    Radius*=0.8; rad*=0.8; //where from?


                    //+Radius-rad=0;
                    //+Radius+rad= (num2-2/4.)*FontSpace

                    alpha=M_PI/NumberOfFrames*i; //current angle of rotation
                    pnt->drawText(offsetX, offsetY, helpString);
                    //q-1 < num2 always
                    pnt->drawText(offsetX+int(Radius-rad*cos(alpha)), offsetY-rad*sin(alpha), QString(HELP[mixNum[q]]));
                    pnt->drawText(offsetX+int(Radius+rad*cos(alpha)), offsetY+rad*sin(alpha), QString(HELP[mixNum[num2]]));
                    ++k;
                    cout<<SavePath(dir, k).toStdString()<<" "<<pic.isNull()<<" "<<pic.save(SavePath(dir, k), 0, 100)<<endl;
                }
            }
            mixNum[num2]=mixNum[q];
            mixNum[q]=q;
            ++k;

            if(videoFlag == 1)
            {
                //pause
                for(int i=0; i<15; ++i)
                {
                    ++k;
                    cout<<SavePath(dir, k).toStdString()<<" "<<pic.isNull()<<" "<<pic.save(SavePath(dir, k), 0, 100)<<endl;
                }
            }


            //last, prelast,...
            for(num1=0; num1<letterNumber; ++num1) //find our letter
            {
                if(mixNum[num1]==letterNumber-q-1) break;
            }

            //a @skeleton@
            helpString=""; //skeleton string
            helpStr=""; //what we have before letter changing
            for(int i=0; i<letterNumber; ++i)
            {
                helpStr.append(HELP[mixNum[i]]);
                if( (i!=letterNumber-q-1) && (i!=num1) )
                {
                    helpString.append(HELP[mixNum[i]]);
                }
                else
                {
                    helpString.append(" ");
                }
            }


            if(helpStr==HELP) break;

            //draw all the jpg-frames
            if(videoFlag == 1)
            {
                for(int i=0; i<NumberOfFrames+1; ++i)
                {
                    if(HELP[mixNum[letterNumber-q-1]] == HELP[mixNum[num1]]) break; //identycal letters exception

                    pic.fill(QColor("black"));

                    Radius=FontSpace*double(letterNumber-q-1+num1)/2.;  //center of rotation
                    rad =  FontSpace*double(letterNumber-q-1-num1)/2.;  //radius of rotation
                    Radius*=0.8; rad*=0.8; //where from?
                    //Radius+rad = (letterNumber-q-3/2)*FontSpace
                    //Radius-rad = (num1-1/2)*FontSpace

                    alpha=M_PI/NumberOfFrames*i; //current angle of rotation
                    pnt->drawText(offsetX, offsetY, helpString);
                    //q-1 < num2 always
                    pnt->drawText(offsetX+int(Radius-rad*cos(alpha)), offsetY-rad*sin(alpha), QString(HELP[mixNum[num1]]));
                    pnt->drawText(offsetX+int(Radius+rad*cos(alpha)), offsetY+rad*sin(alpha), QString(HELP[mixNum[letterNumber-q-1]]));
                    ++k;
                    cout<<SavePath(dir, k).toStdString()<<" "<<pic.isNull()<<" "<<pic.save(SavePath(dir, k), 0, 100)<<endl;
                }
            }
            mixNum[num1]=mixNum[letterNumber-q-1];
            mixNum[letterNumber-q-1]=letterNumber-q-1;

            //pause before next letter change
            //pause for many changes
//            for(int i=0; i<15; ++i)
//            {
//                ++k;
//                cout<<SavePath(dir, k).toStdString()<<" "<<pic.isNull()<<" "<<pic.save(SavePath(dir, k), 0, 100)<<endl;
//            }

            dir->mkdir("LastFrames");
            helpString = dir->absolutePath();
            helpString.append(QDir::separator()).append("LastFrames").append(QDir::separator()).append("anagramm");
            helpString.append(QString::number(AnagrammNumber)).append(".jpg");
            //save/not save last frames
            if(videoFlag == 1) cout<<helpString.toStdString()<<" "<<pic.save(helpString, 0, 100)<<endl;

            break; //change only first&last letters


        }//endof for(q=1...)

        helpString = dir->absolutePath();
        helpString.prepend("cd ");
        helpString.append(" && mencoder \"mf://*.jpg\" -mf type=jpg -o anagramm");
        helpString.append(QString::number(AnagrammNumber)).append(".mpg -ovc lavc -lavcopts vcodec=msmpeg4v2"); //q-current number
        cout<<helpString.toStdString()<<endl<<endl;

        if(videoFlag == 1) system(helpString.toStdString().c_str()); //make video

        QStringList lst = dir->entryList(QStringList("*.jpg"));
        for(int i=0; i<lst.length(); ++i)
        {
            helpString = dir->absolutePath().append(QDir::separator()).append(lst.at(i));
            if(remove(helpString.toStdString().c_str()) != 0)
            {
                perror("cannot delete file");
                return 100500;
            }
        }


        ++AnagrammNumber;
//        if(AnagrammNumber == 1) break;

    } //endof while(!feof)
    outF.close();
	answers.close();

    pnt->end();
    delete pnt;

    return 0;
}
