/*
    05/02/2021
    This program allows user to search for a movie, then, if the movie is found,
    displays its information onto the console, and
    prompts the user to save the movie into a favorites file.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <iomanip>

using namespace std;

struct Movie
{
    string title, director, releaseDate;
    double grossTotal;
    int runTime;

};

int numberOfLines(ifstream &);
void populateMovieFromFile(ifstream &, Movie &);
void displayMovie(const Movie &);
Movie *createDatabase(int &);
bool caseInsensitiveCmp(string, string);
void findMovie(Movie *, int);
void saveToFile(const Movie &);
bool promptToContinue();
void displayFavorites();

int main()
{
    int size;
    ifstream infile;

    Movie *movies = createDatabase(size);
    findMovie(movies, size);
    displayFavorites();
    delete [] movies;
    return 0;
}
//***************************************************
//Receives: ifstream file as reference to access movies.txt
//Returns: The number of lines in movies.txt as an integer
//Purpose: Loops each file line and increments counter for each one
int numberOfLines(ifstream &infile)
{
    int counter = 0;
    string temp;

    while(getline(infile, temp))
    {
        counter++;
    }

    return counter;
}

//***************************************************
//Receives: memory location of movies.txt and the Movie struct
//Returns: Does not return anything
//Purpose: reads each part of a line up to the comma and stores in the Movie struct
void populateMovieFromFile(ifstream &infile, Movie &movies)
{
    string grossTotalTemp, runTimeTemp;

    infile >> ws; //eliminates white space after the comma
    getline(infile, movies.title, ',');
    infile >> ws;
    getline(infile, grossTotalTemp, ',');
    movies.grossTotal = stod(grossTotalTemp); //convert string to double
    infile >> ws;
    getline(infile, movies.director, ',');
    infile >> ws;
    getline(infile, movies.releaseDate, ',');
    infile >> ws;
    getline(infile, runTimeTemp);
    movies.runTime = stoi(runTimeTemp); //convert string to int
}
//***************************************************
//Receives: Movie struct as a const to prevent changes and reference for faster run time
//Returns: No return
//Purpose: Displays the movies' data from the struct
void displayMovie(const Movie &movies)
{
    cout << "\n\n";
    cout << setw(14) << "Title: " << movies.title << endl;
    cout << setw(14) << "Gross Total: " << movies.grossTotal << " billion dollars" << endl;
    cout << setw(14) << "Director: " << movies.director << endl;
    cout << "Release date: " << movies.releaseDate << endl;
    cout << setw(14) << "Runtime: " << movies.runTime << endl;
    cout << "\n\n";
}
//***************************************************
//Receives: The number of lines in the file as reference
//Returns: The dynamic array of Movie struct
//Purpose: Create dynamic array of type Movie and store movie data
Movie *createDatabase(int &size)
{
    Movie *m;
    string fileName;

    do
    {
        cout << "Enter file name: ";
        getline(cin, fileName);
        if(fileName != "movies.txt"){
            cout << "Error: file name must be: 'movies.txt'\n\n";
        }
    }while(fileName != "movies.txt");

    ifstream infile(fileName);

    if(!infile){
        cout << "Error opening file!\n\n";
    }else{
        if(!infile.fail()){
            size = numberOfLines(infile);
            infile.close();
            infile.open(fileName);
            m = new Movie[size];
            for(int i = 0; i < size; i++)
            {
                populateMovieFromFile(infile, m[i]);
            }
            infile.close();
        }
    }
    return m;

}
//***************************************************
//Receives: Two strings for comparison
//Returns: True if words are equal, false if not
//Purpose: Make input not case sensitive
bool caseInsensitiveCmp(string word1, string word2)
{
    string temp = word2;
    if(word1.size() == word2.size())
    {
        for(int i = 0; i < word1.size(); i++)
        {
            if(islower(word1[i])){
                word2[i] = tolower(word2[i]);
            }else if(isupper(word1[i]))
                word2[i] = toupper(word2[i]);
        }
        if(word1 == word2)
            return true;
    }
    return false;
}
//***************************************************
//Receives: Pointer to dynamic array and number of lines in movie.txt
//Returns: No return
//Purpose: Search for movie and its data, and allows save to favorites.
void findMovie(Movie *movies, int numberOfMovies)
{
    ifstream infile;
    char input;
    string movieTitle;

    do{
        infile.open("movies.txt");
        cout << "Enter movie title to search for: ";
        getline(cin, movieTitle);
        numberOfMovies = numberOfLines(infile);
        bool flag = false;
        for(int i = 0; i < numberOfMovies; i++)
        {
            if(caseInsensitiveCmp(movies[i].title, movieTitle)){
                flag = true;
                displayMovie(movies[i]);
                cout << "Save movie to favorites? (Y or N): ";
                cin.get(input);
                cin.ignore();
                if(input == 'Y' || input == 'y'){
                    saveToFile(movies[i]);
                }
                break;
            }
        }
        if(!flag){
            cout << "Error: unable to locate " << "'" << movieTitle << "'\n\n";
        }
        infile.close();
    }while(!promptToContinue());

}
//***************************************************
//Receives: Movie struct as const to prevent changes
//Returns: No return
//Purpose: Saves found movie into favorites.txt
void saveToFile(const Movie &movies)
{
    fstream favFile;
    favFile.open("favorites.txt", ios::in);
    favFile.close();
    if(favFile.fail()){
        cout << "File does not exist. Creating a file called favorites...\n\n";
        favFile.open("favorites.txt", ios::out);
        favFile.close();
    }

    favFile.open("favorites.txt", ios::out | ios::app);
    favFile << movies.title << ',' << " " << movies.grossTotal << ',' << " "
    << movies.director << ',' << " " << movies.releaseDate << ',' << " " << movies.runTime << endl;
    favFile.close();
    cout << "Successfully saved to favorites!\n\n";
}
//***************************************************
//Receives: Nothing
//Returns: True if user enters yes, false if no
//Purpose: Checks user input if its yes or no. Anything else will loop the prompt.
bool promptToContinue()
{
    char input;
    bool flag, repeat;
    do
    {
        cout << "Would you like to exit? (Y/N): ";
        cin.get(input);
        cin.ignore();
        repeat = false;
        switch(input){
            case 'Y':
                flag = true;
                break;
            case 'y':
                flag = true;
                break;
            case 'N':
                flag = false;
                break;
            case 'n':
                flag = false;
                break;
            default:
                repeat = true;
                cout << "Error: invalid input!\n\n";
                break;
        }
    }while(repeat);
    cout << "\n\n";
    return flag;
}
//***************************************************
//Receives: Nothing
//Returns: Nothing
//Purpose: Displays each movie saved in favorites.txt.
void displayFavorites(){
    string title, grossTotal, director, releaseDate, runTime;
    int size, counter = 0;
    ifstream favFile;

    favFile.open("favorites.txt");

    if(!favFile){
        cout << "Error opening file!\n\n";
    }

    size = numberOfLines(favFile);
    favFile.close();

    favFile.open("favorites.txt");
    if(size == 0){
        cout << "No movies saved in favorites.txt.\n\n";
    }else if(size > 0){
        cout << "Your saved movies are: \n\n";
        while(counter < size){
            favFile >> ws; //eliminates white space after the comma
            getline(favFile, title, ',');
            favFile >> ws;
            getline(favFile, grossTotal, ',');
            favFile >> ws;
            getline(favFile, director, ',');
            favFile >> ws;
            getline(favFile, releaseDate, ',');
            favFile >> ws;
            getline(favFile, runTime);

            cout << setw(14) << "Title: " << title << endl;
            cout << setw(14) << "Gross Total: " << grossTotal << " billion dollars" << endl;
            cout << setw(14) << "Director: " << director << endl;
            cout << "Release Date: " << releaseDate << endl;
            cout << setw(14) << "Runtime: " << runTime << endl;
            cout << "\n\n";
            counter++;
        }
    }
    favFile.close();
}

/* Execution Results
Enter file name: movies
Error: file name must be: 'movies.txt'

Enter file name: movies.txt
Enter movie title to search for: interstellar
Error: unable to locate 'interstellar'

Would you like to exit? (Y/N): n


Enter movie title to search for: skyFall


       Title: Skyfall
 Gross Total: 1.109 billion dollars
    Director: Sam Mendes
Release date: 11/9/12
     Runtime: 143


Save movie to favorites? (Y or N): y
File does not exist. Creating a file called favorites...

Successfully saved to favorites!

Would you like to exit? (Y/N): n


Enter movie title to search for: rogue one


       Title: Rogue One
 Gross Total: 1.056 billion dollars
    Director: Gareth Edwards
Release date: 12/16/16
     Runtime: 133


Save movie to favorites? (Y or N): y
Successfully saved to favorites!

Would you like to exit? (Y/N): g
Error: invalid input!

Would you like to exit? (Y/N): y


Your saved movies are:

       Title: Skyfall
 Gross Total: 1.109 billion dollars
    Director: Sam Mendes
Release Date: 11/9/12
     Runtime: 143


       Title: Rogue One
 Gross Total: 1.056 billion dollars
    Director: Gareth Edwards
Release Date: 12/16/16
     Runtime: 133



Process returned 0 (0x0)   execution time : 48.819 s
Press any key to continue.
*/
