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
#include <valarray>


std::vector<std::string> readWordsFile(const QString & filePath)
{
    std::vector<std::string> words;

	std::ifstream fil;
    fil.open(filePath.toStdString());

	if(fil.bad())
	{
		return {};
	}

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

	std::ofstream feel;
    feel.open(filePath.toStdString());

    for(auto str : words)
    {
		feel << str << std::endl;
    }
    feel.close();
}

std::ostream & operator << (std::ostream &os, const QString & toOut)
{
    os << toOut.toStdString();
    return os;
}

template <template <typename> class Cont, typename Typ>
std::ostream & operator << (std::ostream &os, const Cont<Typ> & toOut)
{
	for(Typ in : toOut)
	{
		os << in << " ";
	}
	return os;
}
template std::ostream & operator<<  (std::ostream & os, const std::valarray<int> & toOut);

QString rigtNum(int input, int N = 3) // prepend zeros
{
    QString h;
    h.setNum(input);
    h.prepend(QString(N, '0'));
    return h.right(N);
}

QString SavePath(QDir * dir, int &i)
{
    QString h = dir->absolutePath();
	h += "/anagramm" + rigtNum(i) + ".jpg";
    return h;
}

QString mixWord(const QString & inWord)
{
	std::valarray<int> mixNum(inWord.size());
	std::iota(std::begin(mixNum),
			  std::end(mixNum),
			  0);

	int num = 0;

	do
	{
		std::shuffle(std::begin(mixNum),
					 std::end(mixNum),
					 std::default_random_engine(
						 std::chrono::system_clock::now().time_since_epoch().count()));

		/// 1 if next letter is from right order
		/// -1 if prev
		decltype(mixNum) nexts = mixNum.cshift(1) - mixNum;

		num = 0;
		for(int i = 0; i < mixNum.size() - 1; ++i)
		{
			if(std::abs(nexts[i]) == 1) { num += 1; }
		}

		if(mixNum[0] == 0 || mixNum[mixNum.size() - 1] == mixNum.size() - 1)
		{
			num += 10;
		}

//		std::cout << mixNum << "\t"
//				  << nexts << "\t"
//				  << num << "\t"
//				  << std::endl;
	} while (num > 0);

	QString res = inWord;
	for(int i = 0; i < inWord.size(); ++i)
	{
		res[i] = inWord[mixNum[i]];
	}
	return res;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    double FontSpace = 120.;
	double offsetX, offsetY;

	QString helpString;
    QPixmap pic(1280, 1024);
	QPainter pnt;
	QDir * dir = new QDir();
	pic.fill(QColor("gray"));
	pnt.begin(&pic);

    dir->cd("/media/Files"); //Michael

    dir->mkdir("Pictures");
    dir->cd("Pictures");
    dir->mkdir("Anagramms");
    dir->cd("Anagramms");
    dir->mkdir("FirstFrames");
    dir->mkdir("LastFrames");


	QFont myFont = QFont("Courier", FontSpace, QFont::Normal);

	pnt.setFont(myFont);
	pnt.setPen("lightgray");

	int AnagrammNumber = 1;

    std::vector<std::string> wordList
			= readWordsFile("../AnagrammAnim/6letNewest.txt");
	/// shuffle anagramms - no need
	std::shuffle(std::begin(wordList),
				 std::end(wordList),
				 std::default_random_engine(
					 std::chrono::system_clock::now().time_since_epoch().count()));


	std::ofstream answers("/media/Files/Pictures/Anagramms/answers.txt");

	QString initWord;
	QString mixedWord;
    for(const std::string & word : wordList)
    {
		initWord = QString(word.c_str()); /// ow

		int wordWidth = QFontMetrics(myFont).width(initWord);
		int letterHeight = QFontMetrics(myFont).xHeight();
		offsetX = pic.width() / 2 - wordWidth / 2;
		offsetY = pic.height() / 2.0 + letterHeight / 2;

		mixedWord = mixWord(initWord);

		std::cout << initWord << "\t" << mixedWord << std::endl;

		pic.fill(QColor("black"));

        //draw the first frame
		pnt.drawText(offsetX, offsetY, mixedWord);

		helpString = dir->absolutePath()
					 + "/FirstFrames"
					 + "/anagramm_"
					 + rigtNum(AnagrammNumber, 3) + ".jpg";

		pic.save(helpString, 0, 100);



//		if(AnagrammNumber == 15) break;

        ++AnagrammNumber; continue; // only FirstFrames












#if 0
		/// video
        k=0; //number of current jpg-frame
        for(q=0; q<letterNumber/2+1; ++q) //begin N iterations of back-mixing
        {
            //first, second,...
            for(num2=0; num2<letterNumber; ++num2) //search the first(second,...) letter
            {
                if(mixNum[num2]==q) break;
            }

            //a @skeleton@
			helpString.clear(); //skeleton string
			helpStr.clear(); //what we have before letter changing
			for(int i = 0; i < letterNumber; ++i)
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
					pnt.drawText(offsetX, offsetY, helpString);
                    //q-1 < num2 always
					pnt.drawText(offsetX+int(Radius-rad*cos(alpha)), offsetY-rad*sin(alpha), QString(HELP[mixNum[q]]));
					pnt.drawText(offsetX+int(Radius+rad*cos(alpha)), offsetY+rad*sin(alpha), QString(HELP[mixNum[num2]]));
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
					pnt.drawText(offsetX, offsetY, helpString);
                    //q-1 < num2 always
					pnt.drawText(offsetX+int(Radius-rad*cos(alpha)), offsetY-rad*sin(alpha), QString(HELP[mixNum[num1]]));
					pnt.drawText(offsetX+int(Radius+rad*cos(alpha)), offsetY+rad*sin(alpha), QString(HELP[mixNum[letterNumber-q-1]]));
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

		helpString = "cd " + dir->absolutePath() + " && mencoder \"mf://*.jpg\" -mf type=jpg -o anagramm"
					 + QString::number(AnagrammNumber) + ".mpg -ovc lavc -lavcopts vcodec=msmpeg4v2";
		std::cout << helpString.toStdString() << std::endl;

        if(videoFlag == 1) system(helpString.toStdString().c_str()); //make video

        QStringList lst = dir->entryList(QStringList("*.jpg"));
		for(const QString & fileName : lst)
        {
			helpString = dir->absolutePath() + "/" + fileName;
            if(remove(helpString.toStdString().c_str()) != 0)
            {
                perror("cannot delete file");
                return 100500;
            }
        }


        ++AnagrammNumber;
//        if(AnagrammNumber == 1) break;
#endif

	}
	answers.close();
	pnt.end();
    return 0;
}
