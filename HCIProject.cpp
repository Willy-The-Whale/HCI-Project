//HCI Project

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;

//class for a row of data. They're all int for the arbitrary code part
class dataInstance
{
public:
		int age = 0;
		int employment = 0;
		int third = 0;
		int education = 0;
		int fifth = 0;
		int marital = 0;
		int profession = 0;
		int family = 0;
		int race = 0;
		int gender = 0;
		int eleventh = 0;
		int twelve = 0;
		int hoursWorked = 0;
		int birthCountry = 0;
		int income = 0;

		int get(int num)
		{
			switch (num)
			{
			case 1: return age;
				break;
			case 2: return employment;
			case 3: return third;
			case 4: return education;
			case 5: return fifth;
			case 6: return marital;
			case 7: return profession;
			case 8: return family;
			case 9: return race;
			case 10: return gender;
			case 11: return eleventh;
			case 12: return twelve;
			case 13: return hoursWorked;
			case 14: return birthCountry;
			case 15: return income;
			default: return -1;
				break;
			}
		}

	
};

class unique
{
public:
	int value = 0;
	int Counter1 = 0; //number of occurrences with the <=50k attribute
	int Counter2 = 0; //number of occurrences with the >50k attribute
	float probability1 = 0.0; //probability of this value with the <=50k attribute
	float probability2 = 0.0; //probability of this value with the >50k attribute

	void count(int num)
	{
		if (num == 1)
		{
			Counter1++;
		}
		else if (num == 2)
		{
			Counter2++;
		}
	}
};

//Global Variable
int TOTAL = 16281; //Use this to modify how much of the data gets read for convenience. The actual total is 16281.
bool AVOID = false;


//Function Prototype
void dataAdd(dataInstance& line, int dataNum, string input);
void display(dataInstance line);

int main()
{
	srand(time(0));
	vector<dataInstance> totalArr;
	vector<dataInstance> thirtyArr;
	vector<dataInstance> seventyArr;

	vector<unique> uniqueArr[15];
	int* entropyResult;
	
	dataInstance temp;
	unique temp2;
	fstream inputfile;
	ofstream outputFile;
	int seventyCount = 0;
	int thirtyCount = 0;
	int tempNum = 0;
	int num = -1;
	
	string line = "";
	string part = "";
	

	inputfile.open("human.csv");
	if (!inputfile.is_open())
	{
		std::cout << "Unable to open file." << endl;
		exit(1);
	}
	outputFile.open("Result.txt");
	if (!outputFile.is_open())
	{
		std::cout << "Unable to open output file." << endl;
		exit(1);
	}
	//Loop skips the header row since it isn't actual data.
	for (int j = 0; j < 15; j++)
	{
		if (j == 14)
		{
			getline(inputfile, part);
		}
		else
		{
			getline(inputfile, part, ',');
		}
	}
	for (int i = 0; i < TOTAL; i++)
	{
		AVOID = false;
		for(int j = 0; j < 15;j++)
		{
			if (j == 14)
			{ //stops at the newline character at the end of a row
				getline(inputfile, part);
			}
			else
			{
				getline(inputfile, part, ',');
				
			}
			//converts the input into arbitrary numbers
			dataAdd(temp, j, part);

		}
		
		if (!AVOID) //Avoids the rows with ? in it
		{
			totalArr.push_back(temp);
		}
	}
	//splitting up the data
	tempNum = totalArr.size();
	seventyCount = floor((float)tempNum * .70); //math functions for the result to be integers
	thirtyCount = ceil((float)tempNum * .30);

	std::cout << "Total size of data: " << tempNum << endl;
	outputFile << "Total size of data: " << tempNum << endl;
	std::cout << "Size of 70% of the data: " << seventyCount << endl;
	outputFile << "Size of 70% of the data: " << seventyCount << endl;
	std::cout << "Size of 30% of the data: " << thirtyCount << endl;
	outputFile << "Size of 30% of the data: " << thirtyCount << endl;
	bool flag = false;
	bool flag2 = false;
	for (int i = 0; i < tempNum; i++) //Splits the data into 70/30 using random numbers
	{
		num = rand() % 2; //70% is num = 1, 30% is num = 0
		if (num)
		{
			seventyArr.push_back(totalArr[0]);
			if (seventyArr.size() == seventyCount) //Ends the loop prematurely if one split is filled up.
			{
				tempNum = i;
				flag = true;
			}
		}
		else
		{
			thirtyArr.push_back(totalArr[0]);
			if (thirtyArr.size() == thirtyCount)
			{
				flag2 = true;
				tempNum = i;
			}
		}
		totalArr.erase(totalArr.begin()); //deletes the front value in the total array that was just moved into one of the splits
		
		
	}

	//Adds the remaining values into the split that isn't filled up
	if (flag)
	{
		while (!totalArr.empty())
		{
			thirtyArr.push_back(totalArr[0]);
			totalArr.erase(totalArr.begin());
		}
	}
	else if (flag2)
	{
		while (!totalArr.empty())
		{
			seventyArr.push_back(totalArr[0]);
			totalArr.erase(totalArr.begin());
		}
	}

	//used to hold entropy values
	entropyResult = new int[thirtyCount];

	/*
	cout << "30% dataset: " << endl;
	for (int i = 0; i < thirtyCount; i++)
	{
		display(thirtyArr[i]);
	}
	cout << "70% dataset: " << endl;
	for (int i = 0; i < seventyCount; i++)
	{
		display(seventyArr[i]);
	}
	*/

	//Counts and records every unique value with how many times it appeared and which attribute it belonged to (<=50k or >50k).
	bool isUnique = true;
	int uniqueCounter = 0;
	for (int j = 0; j < seventyCount; j++) 
	{
		for (int i = 0; i < 15; i++)
		{
			temp2.value = seventyArr[j].get(i+1);
			while (uniqueCounter < uniqueArr[i].size() && isUnique) //checks if it's unique
			{
				if (temp2.value == uniqueArr[i][uniqueCounter].value) //checks if it's already in our unique array
				{
					isUnique = false;
					uniqueArr[i][uniqueCounter].count(seventyArr[j].get(15));
				}
				uniqueCounter++;
			}
			if (isUnique) //creates a new unique entry in the array and logs the attribute
			{
				uniqueArr[i].push_back(temp2);
				uniqueArr[i][uniqueArr[i].size() - 1].count(seventyArr[j].get(15)); //increments the counter for the unique value
			}
			isUnique = true;
			uniqueCounter = 0;

		}
	}

	//Displaying/storing the probabilities
	
	uniqueCounter = 0;
	int top = 0;

	int bottom1 = uniqueArr[14][0].Counter1;
	int bottom2 = uniqueArr[14][1].Counter2;
	if (bottom1 == 0) //We don't know for certain which will have the total number of >50k or <=50k so we have to check
	{
		bottom2 = uniqueArr[14][0].Counter2;
		bottom1 = uniqueArr[14][1].Counter1;
	}

	float result = 0.0;

	for (int i = 0; i < 14; i++) //doesn't display the last column since that's the income one. The one that we're testing with.
	{
		if (i != 2) { //Doesn't display the third column because that's the one with a lot of unique values and it just floods the console.
			while (uniqueCounter < uniqueArr[i].size())
			{
				std::cout << "The probability of value " << uniqueArr[i][uniqueCounter].value << " of column " << i+1 << " for <=50k:";
				outputFile << "The probability of value " << uniqueArr[i][uniqueCounter].value << " of column " << i + 1 << " for <=50k:";
				top = uniqueArr[i][uniqueCounter].Counter1;
				result = float(top) / float(bottom1);
				
				uniqueArr[i][uniqueCounter].probability1 = result;
				std::cout << result;
				outputFile << result;

				std::cout << endl << "The probability of value " << uniqueArr[i][uniqueCounter].value << " of column " << i+1 << " for >50k:";
				outputFile << endl << "The probability of value " << uniqueArr[i][uniqueCounter].value << " of column " << i + 1 << " for >50k:";
				top = uniqueArr[i][uniqueCounter].Counter2;
				result = float(top) / float(bottom2);
				std::cout << result;
				std::cout << endl;
				outputFile << result << endl;

				uniqueArr[i][uniqueCounter].probability2 = result;
				uniqueCounter++;
			}
			uniqueCounter = 0;
		}
	}
	std::cout << "Number of <=50k's: " << bottom1 << endl;
	std::cout << "Number of >50k's: " << bottom2 << endl;
	outputFile << "Number of <=50k's: " << bottom1 << endl;
	outputFile << "Number of >50k's: " << bottom2 << endl;
	int target = 0;
	isUnique = true;
	float sum1 = 0.0;
	float sum2 = 0.0;
	float tempNum1 = 0.0;
	float tempNum2 = 0.0;
	float accuracy = 0;

	//Entropy calculation
	for (int i = 0; i < thirtyCount; i++)
	{
		for (int j = 0; j < 14; j++)
		{

			target = thirtyArr[i].get(j + 1);
			while (isUnique && uniqueCounter < uniqueArr[j].size()) //checks if data from the 30% was in the 70%.
			{
				if (target == uniqueArr[j][uniqueCounter].value)
				{
					isUnique = false;
				}
				if(isUnique)
					uniqueCounter++;
			}
			if (!isUnique) //If it was found in the testing data, extract it's probability into the formula, otherwise, ignore it/treat it like a 0
			{
				tempNum1 = uniqueArr[j][uniqueCounter].probability1;
				tempNum2 = uniqueArr[j][uniqueCounter].probability2;
				sum1 += (log2(tempNum1));
				sum2 += (log2(tempNum2));
			}
			uniqueCounter = 0;
			isUnique = true;
		}
		sum1 *= (static_cast<float>(-1) / 14); //the -1 / N part, N being the number of columns not including income since we're pretending we don't have it
		sum2 *= (static_cast<float>(-1) / 14);
		if (sum1 > sum2) //case for >50k
		{
			entropyResult[i] = 2;
		}
		else if (sum2 >= sum1) //case for <= 50k
		{
			entropyResult[i] = 1;
		}
		sum1 = 0.0;
		sum2 = 0.0;
		if (entropyResult[i] == thirtyArr[i].income) //increments if our prediction is correct
		{
			accuracy++;
		}
	}
	std::cout << "Accuracy: " << accuracy << endl;
	std::cout << "Size of testing data: " << thirtyCount << endl;
	outputFile << "Accuracy: " << accuracy << endl;
	outputFile << "Size of testing data: " << thirtyCount << endl;
	tempNum1 = accuracy / (float)thirtyCount;
	tempNum1 *= 100.0;
	std::cout << endl << "The system has an accuracy of " <<tempNum1 << "%" << endl;
	outputFile << endl << "The system has an accuracy of " << tempNum1 << "%" << endl;
	
	std::system("pause");
	inputfile.close();
	outputFile.close();
	delete[] entropyResult;
}

void dataAdd(dataInstance& line, int dataNum, string input)
{
	int result;
	float tempNum = 0.0;
	if (input.find('?') != -1) //flags the row so it won't be added to the total data
	{
		AVOID = true;
	}
	else {
		switch (dataNum) {
		case 0: result = stoi(input);
			line.age = result;
			break;
		case 1: if (input == " Private")
		{
			result = 1;
		}
			 else if (input == " Local-gov")
		{
			result = 2;
		}
			 else if (input == " Self-emp-not-inc")
		{
			result = 3;
		}
			 else if (input == " Federal-gov")
		{
			result = 4;
		}
			 else if (input == " State-gov")
		{
			result = 5;
		}
			 else if (input == " Self-emp-inc")
		{
			result = 6;
		}
			 else if (input == " Never-worked")
		{
			result = 7;
		}
			 else if (input == " Without-pay")
		{
			result = 8;
		}
			 else
		{
			std::cout << "Error in employment. It's :" << input<< endl;
			result = 9;
		}
			  line.employment = result;
			break;
		case 2: result = stoi(input); 
			line.third = result;
			break;
		case 3: if (input == " Masters")
		{
			result = 1;
		}
			  else if (input == " Some-college")
		{
			result = 2;
		} 
			  else if (input == " HS-grad")
		{
			result = 3;
		}
			  else if (input == " Assoc-acdm")
		{
			result = 4;
		}
			  else if (input == " 11th")
		{
			result = 5;
		}
			  else if (input == " 10th")
		{
			result = 6;
		}
			  else if (input == " Prof-school")
		{
			result = 7;
		}
			  else if (input == " 7th-8th")
		{
			result = 8;
		}
			  else if (input == " Bachelors")
		{
			result = 9;
		}
			  else if (input == " Doctorate")
		{
			result = 10;
		}
			  else if (input == " 5th-6th")
		{
			result = 11;
		}
			  else if (input == " Assoc-voc")
		{
			result = 12;
		}
			  else if (input == " 9th")
		{
			result = 13;
		}
			  else if (input == " 12th")
		{
			result = 14;
		}
			  else if (input == " 1st-4th")
		{
			result = 15;
		}
			  else if (input == " Preschool")
		{
			result = 16;
		}
			  else
		{
			std::cout << "Error in education. It is: " << input << endl;
			result = 17;
		}
			  line.education = result;
			break;
		case 4: result = stoi(input);
			line.fifth = result;
			break;
		case 5: if (input == " Never-married")
		{
			result = 1;
		}
			  else if (input == " Married-civ-spouse")
		{
			result = 2;
		}
			  else if (input == " Widowed")
		{
			result = 3;
		}
			  else if (input == " Divorced")
		{
			result = 4;
		}
			  else if (input == " Separated")
		{
			result = 5;
		}
			  else if (input == " Married-spouse-absent")
		{
			result = 6;
		}
			  else if (input == " Married-AF-spouse")
		{
			result = 7;
		}
			  else 
		{
			std::cout << "Error in Marital. It's: " << input << endl;
			result = 8;
		}
			  line.marital = result;
			break;
		case 6: if (input == " Adm-clerical")
		{
			result = 1;
		}
			  else if (input == " Armed-Forces")
		{
			result = 2;
		}
			  else if (input == " Craft-repair")
		{
			result = 3;
		}
			  else if (input == " Exec-managerial")
		{
			result = 4;
		}
			  else if (input == " Farming-fishing")
		{
			result = 5;
		}
			  else if (input == " Handlers-cleaners")
		{
			result = 6;
		}
			  else if (input == " Machine-op-inspct")
		{
			result = 7;
		}
			  else if (input == " Other-service")
		{
			result = 8;
		}
			  else if (input == " Priv-house-serv")
		{
			result = 9;
		}
			  else if (input == " Prof-specialty")
		{
			result = 10;
		}
			  else if (input == " Protective-serv")
		{
			result = 11;
		}
			  else if (input == " Sales")
		{
			result = 12;
		}
			  else if (input == " Tech-support")
		{
			result = 13;
		}
			  else if (input == " Transport-moving")
		{
			result = 14;
		}
			  else
		{
			std::cout << "Error in profession. It's:" << input << endl;
			result = 15;
		}
			  line.profession = result;
			break;
		case 7: if (input == " Husband")
		{
			result = 1;
		}
			  else if (input == " Not-in-family")
		{
			result = 2;
		}
			  else if (input == " Other-relative")
		{
			result = 3;
		}
			  else if (input == " Own-child")
		{
			result = 4;
		}
			  else if (input == " Unmarried")
		{
			result = 5;
		}
			  else if (input == " Wife")
		{
			result = 6;
		}
			  else
		{
			std::cout << "Error in family. It's: " << input << endl;
			result = 7;
		}
			  line.family = result;
			break;
		case 8: if (input == " White")
		{
			result = 1;
		}
			  else if (input == " Other")
		{
			result = 2;
		}
			  else if (input == " Black")
		{
			result = 3;
		}
			  else if (input == " Asian-Pac-Islander")
		{
			result = 4;
		}
			  else if (input == " Amer-Indian-Eskimo")
		{
			result = 5;
		}
			  else
		{
			std::cout << "Error in race. It's: " << input << endl;
			result = 6;
		}
			  line.race = result;
			break;
		case 9: if (input == " Female")
		{
			result = 1;
		}
			  else if (input == " Male")
		{
			result = 2;
		}
			  else
		{
			std::cout << "Error in gender. It's: " << input << endl;
			result = 3;
		}
			  line.gender = result;
			break;
		case 10: result = stoi(input); 
		
			line.eleventh = result;
			break;
		case 11: result = stoi(input); 
			line.twelve = result;
			break;
		case 12: result = stoi(input); 
			line.hoursWorked = result;
			break;
		case 13: if (input == " United-States")
		{
			result = 1;
		}
			   else if (input == " Yugoslavia")
		{
			result = 2;
		}
			   else if (input == " Vietnam")
		{
			result = 3;
		}
			   else if (input == " Trinadad&Tobago")
		{
			result = 4;
		}
			   else if (input == " Thailand")
		{
			result = 5;
		}
			   else if (input == " Taiwan")
		{
			result = 6;
		}
			   else if (input == " South")
		{
			result = 7;
		}
			   else if (input == " Scotland")
		{
			result = 8;
		}
			   else if (input == " Puerto-Rico")
		{
			result = 9;
		}
			   else if (input == " Portugal")
		{
			result = 10;
		}
			   else if (input == " Poland")
		{
			result = 11;
		}
			   else if (input == " Philippines")
		{
			result = 12;
		}
			   else if (input == " Peru")
		{
			result = 13;
		}
			   else if (input == " Outlying-US(Guam-USVI-etc)")
		{
			result = 14;
		}
			   else if (input == " Nicaragua")
		{
			result = 15;
		}
			   else if (input == " Mexico")
		{
			result = 16;
		}
			   else if (input == " Laos")
		{
			result = 17;
		}
			   else if (input == " Japan")
		{
			result = 18;
		}
			   else if (input == " Jamaica")
		{
			result = 19;
		}
			   else if (input == " Italy")
		{
			result = 20;
		}
			   else if (input == " Ireland")
		{
			result = 21;
		}
			   else if (input == " Iran")
		{
			result = 22;
		}
			   else if (input == " India")
		{
			result = 23;
		}
			   else if (input == " Hungary")
		{
			result = 24;
		}
			   else if (input == " Hong")
		{
			result = 25;
		}
			   else if (input == " Honduras")
		{
			result = 26;
		}
			   else if (input == " Haiti")
		{
			   result = 27;
		}
			   else if (input == " Guatemala")
			   {
			   result = 28;
 }
			   else if (input == " Greece")
			   {
			   result = 29;
 }
			   else if (input == " Germany")
			   {
			   result = 30;
 }
			   else if (input == " France")
			   {
			   result = 31;
 }
			   else if (input == " England")
			   {
			   result = 32;
 }
			   else if (input == " El-Salvador")
			   {
			   result = 33;
 }
			   else if (input == " Ecuador")
			   {
			   result = 34;
 }
			   else if (input == " Dominican-Republic")
			   {
			   result = 35;
 }
			   else if (input == " Cuba")
			   {
			   result = 36;
 }
			   else if (input == " Columbia")
			   {
			   result = 37;
				}
			   else if (input == " China")
			   {
			   result = 38;
				}
			   else if (input == " Canada")
			   {
			   result = 39;
				}
			   else if (input == " Cambodia")
			   {
			   result = 40;
				}
			   else
			   {
				   std::cout << "Error in birthCountry. It's: " << input << endl;
			   result = 41;
				}
				line.birthCountry = result;
			break;
		case 14: if (input == " <=50K.")
		{
			result = 1;
		}
			   else if (input == " >50K.")
		{
			result = 2;
		}
			   else
		{
			std::cout << "Error in income. It's: " << input << endl;
			result = 3;
		}
			   line.income = result;
			break;

		default: std::cout << "Default Error." << endl;


		}
	}
}



void display(dataInstance line)
{
	std::cout << "---------------------------------------------" << endl;
	std::cout << "Age: " << line.age << endl;
	std::cout << "Employment: " << line.employment << endl;
	std::cout << "Third col. : " << line.third << endl;
	std::cout << "Education: " << line.education << endl;
	std::cout << "fifth: " << line.fifth << endl;
	std::cout << "Marital: " << line.marital << endl;
	std::cout << "Profession: " << line.profession << endl;
	std::cout << "Family: " << line.family << endl;
	std::cout << "Race: " << line.race << endl;
	std::cout << "Gender: " << line.gender << endl;
	std::cout << "Eleventh col. : " << line.eleventh << endl;
	std::cout << "Twelveth col. : " << line.twelve << endl;
	std::cout << "Hours worked: " << line.hoursWorked << endl;
	std::cout << "Birth Country: " << line.birthCountry << endl;
	std::cout << "Income: " << line.income << endl;
	std::cout << "---------------------------------------------" << endl;
}