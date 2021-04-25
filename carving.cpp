
/*
    Name: Michael Ameteku
    Project: Seam Carving
    Class: Algorithms
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

double calculateEnergy(int **values, int row, int column, int maxRow, int maxColumn);

void getVCumValues(int **energyArray, int **, int maxColumn, int maxRow);

void takeVSeam(int **originalValues, int **cumulativeEnergies, int maxRow, int maxColumn);

void takeHSeam(int **originalValues, int **cumulativeEnergies, int maxRow, int maxColumn);

void getCumHValues(int **energyArray, int **cumulativeEnergyMap, int maxColumn, int maxRow);

void writeToFile(int **fileItems, int initialMaxRow, int initialMaxColumn, std::string fileName);

int main(int argc, char *argv[])
{
    std::ifstream in;
    int picture;
    std::vector<std::vector<int>> rows;

    if (argc == 4)
    {
        std::string fileName = argv[1];
        int vertical = atoi(argv[2]);
        int horizontal = atoi(argv[3]);

        // opening file
        in.open(fileName);

        if (in.fail())
        {
            std::cout << "File not found\n";
            return 0;
        }
        else
        {
            std::string strings;
            getline(in, strings); // getting in the first letter
            in.get();
            getline(in, strings); // getting in the credits

            int maxRow = 0;
            int maxColumn = 0;
            int maxColor = 0;
            in >> maxColumn; // reading in the max value
            in >> maxRow;
            in >> maxColor;

            std::cout << "Max row: " << maxRow << std::endl
                      << "Max Column: " << maxColumn
                      << "\nMax color: " << maxColor << std::endl;

           // std::cout << "About to create points\n";

            // creating array to hold points
            int **points = new int *[maxRow];
            for (int i = 0; i < maxRow; i++)
            {
                //std::cout << "Creating..";
                points[i] = new int[maxColumn];
            }

            // std::cout << "Done with creating array";

            // loading the values of the image in
            std::string row;
            int num;
            getline(in, row);

            for (int i = 0; i < maxRow && !in.eof(); i++)
            {
                for (int a = 0; a < maxColumn; a++)
                {
                    in >> points[i][a];
                }
            }
            //std::cout << "Done \n";
            in.close();

            // remove all vertical seams
            for (int a = 0; a < vertical; a++)
            {
               // std::cout << "Starting vertical seams";
                //create new array from dimensions for energies and another to store the best next directions
                int **energies = new int *[maxRow];
                int **cumulativeEnergies = new int *[maxRow];

                int i;
                for (i = 0; i < maxRow; i++)
                {
                    energies[i] = new int[maxColumn];
                    cumulativeEnergies[i] = new int[maxColumn];
                }

                //populate using the formula : e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|
                for (i = 0; i < maxRow; i++)
                {
                    int energy;
                    for (int a = 0; a < maxColumn; a++)
                    {
                        energy = calculateEnergy(points, i, a, maxRow, maxColumn);
                        // std::cout << energy << " ";
                        energies[i][a] = energy;
                    }
                }

                //get calculate cumulative values
                getVCumValues(energies, cumulativeEnergies, maxColumn, maxRow);

                // remomve the seams
                takeVSeam(points, cumulativeEnergies, maxRow, maxColumn);
                maxColumn--;

                for (int a = 0; a < maxRow; a++)
                {
                    delete[] cumulativeEnergies[a];
                    delete[] energies[a];
                }

                delete[] energies;
                delete[] cumulativeEnergies;
            }

           // std::cout << "Done with verticals\n";

           // remove all horizontal seams
            for (int a = 0; a < horizontal; a++)
            {

               //std::cout << "IN horizontal\n";
                //create new array from dimensions for energies and another to store the best next directions
                int **energies = new int *[maxRow];
                int **cumulativeEnergies = new int *[maxRow];

                int i;
                for (i = 0; i < maxRow; i++)
                {
                    energies[i] = new int[maxColumn];
                    cumulativeEnergies[i] = new int[maxColumn];
                }

                //std::cout << "Done creating dynaimc arrays\n";

                //populate using the formula : e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|
                for (i = 0; i < maxRow; i++)
                {
                    int energy;
                    for (int a = 0; a < maxColumn; a++)
                    {
                        energy = calculateEnergy(points, i, a, maxRow, maxColumn);
                        
                        energies[i][a] = energy;
                    }
                }

                //std::cout << "Done populating dynaimc arrays\n";
                //get calculate cumulative values
                getCumHValues(energies, cumulativeEnergies, maxColumn, maxRow);
               // std::cout << "Done getting cum energies\n";

                // remomve the seams
                takeHSeam(points, cumulativeEnergies, maxRow, maxColumn);
                maxRow--;

               for (int a = 0; a < maxRow; a++)
                {
                    delete[] cumulativeEnergies[a];
                    delete[] energies[a];
                }

                delete[] energies;
                delete[] cumulativeEnergies;
            }
            //std::cout << "Done with horizontal\n";

            writeToFile(points, maxRow, maxColumn, fileName);
            std::cout << "Finished Carving\n";
        }
    }

    return 0;
}

// the formula : e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|
double calculateEnergy(int **values, int row, int column, int maxRow, int maxColumn)
{
    double value;
    int mainValue = values[row][column];

    if (row == 0)
    {
        if (column == 0)
        {
            value = std::abs(mainValue - values[row + 1][column]) + std::abs(mainValue - values[row][column + 1]);
            return value;
        }
        else if (column == maxColumn - 1)
        {
            value = std::abs(mainValue - values[row + 1][column]) + std::abs(mainValue - values[row][column - 1]);
            return value;
        }
        else if (column > 0 && column < maxColumn - 1)
        {
            value = std::abs(mainValue - values[row + 1][column]) + std::abs(mainValue - values[row][column + 1]) + std::abs(mainValue - values[row][column - 1]);
            return value;
        }
    }
    else if (row == maxRow - 1)
    {
        if (column == 0)
        {
            value = std::abs(mainValue - values[row - 1][column]) + std::abs(mainValue - values[row][column + 1]);
            return value;
        }
        else if (column == maxColumn - 1)
        {
            value = std::abs(mainValue - values[row - 1][column]) + std::abs(mainValue - values[row][column - 1]);
            return value;
        }
        else if (column > 0 && column < maxColumn - 1)
        {
            value = std::abs(mainValue - values[row - 1][column]) + std::abs(mainValue - values[row][column + 1]) + std::abs(mainValue - values[row][column - 1]);
            return value;
        }
    }
    else
    {
        if (column > 0 && column < maxColumn - 1)
        {
            value = std::abs(mainValue - values[row + 1][column]) + std::abs(mainValue - values[row][column + 1]) + std::abs(mainValue - values[row - 1][column]) + std::abs(mainValue - values[row][column - 1]);
            return value;
        }
        else if (column == maxColumn - 1)
        {
            value = std::abs(mainValue - values[row - 1][column]) + std::abs(mainValue - values[row + 1][column]) + std::abs(mainValue - values[row][column - 1]);
            return value;
        }
        else
        {
            value = std::abs(mainValue - values[row - 1][column]) + std::abs(mainValue - values[row + 1][column]) + std::abs(mainValue - values[row][column + 1]);
            return value;
        }
    }
    return 1000;
}


// calculating the cumulativeENergy values and placing it in the energy array
void getVCumValues(int **energyArray, int **cumulativeEnergyMap, int maxColumn, int maxRow)
{
    int i;
    int lowestValue;
    int lowestValuePosition;

    //getting the cumulative energy map
    for (i = maxRow - 1; i > -1; i--)
    {
        for (int a = 0; a < maxColumn; a++)
        {

            /*
             row == max
             row < max, col = 0
             row < max, col = max
             row < max, 0 < col < max
             */

            if (i == maxRow - 1)
            {
                cumulativeEnergyMap[i][a] = energyArray[i][a];
            }
            else if (i < maxRow - 1)
            {
                if (a == 0)
                {
                    lowestValue = std::min(cumulativeEnergyMap[i + 1][a], cumulativeEnergyMap[i + 1][a + 1]);
                    cumulativeEnergyMap[i][a] = lowestValue + energyArray[i][a];
                }
                else if (a == maxColumn - 1)
                {
                    cumulativeEnergyMap[i][a] = std::min(cumulativeEnergyMap[i + 1][a], cumulativeEnergyMap[i + 1][a - 1]) + energyArray[i][a];
                }
                else
                {
                    cumulativeEnergyMap[i][a] = std::min(std::min(cumulativeEnergyMap[i + 1][a], cumulativeEnergyMap[i + 1][a - 1]), cumulativeEnergyMap[i + 1][a + 1]) + energyArray[i][a];
                }
            }
        }
    }
}

// used to remove a vertical seam
void takeVSeam(int **originalValues, int **cumulativeEnergies, int maxRow, int maxColumn)
{

    int minumumPosition = 0;
    int minimumEnergy = cumulativeEnergies[0][minumumPosition];

    //finding the first minimum energy on top row
    for (int a = 0; a < maxColumn; a++)
    {
        if (minimumEnergy >= cumulativeEnergies[0][a])
        {
            minumumPosition = a;
            minimumEnergy = cumulativeEnergies[0][minumumPosition];
        }
        //std::cout << "Finding lowest origin: " << minimumEnergy << " at: " << minumumPosition << std::endl;
    }

    //deleting the first min value
    for (int a = minumumPosition; a < maxColumn - 1; a++)
    {
        originalValues[0][a] = originalValues[0][a + 1];
    }

    // findng rest of path
    for (int i = 1; i < maxRow; i++)
    {

        // check if current row is between max and 1
        if (i <= maxRow - 1)
        {
            //checking for left column edge case
            if (minumumPosition == 0)
            {
                if (cumulativeEnergies[i][minumumPosition] < cumulativeEnergies[i][minumumPosition + 1])
                    minumumPosition = minumumPosition;
                else
                    minumumPosition = minumumPosition + 1;
            }

            //checking for right column edge case
            else if (minumumPosition == maxColumn - 1)
            {
                if (cumulativeEnergies[i][minumumPosition] < cumulativeEnergies[i][minumumPosition - 1])
                    minumumPosition = minumumPosition - 1;
            }

            //checkinf for middle cases
            else if (minumumPosition > 0 && minumumPosition < maxColumn - 1)
            {

                if (cumulativeEnergies[i][minumumPosition] < cumulativeEnergies[i][minumumPosition - 1])
                {
                    if (cumulativeEnergies[i][minumumPosition] < cumulativeEnergies[i][minumumPosition + 1])
                        minumumPosition = minumumPosition;
                    else
                        minumumPosition = minumumPosition + 1;
                }
                else if (cumulativeEnergies[i][minumumPosition - 1] < cumulativeEnergies[i][minumumPosition])
                {
                    if (cumulativeEnergies[i][minumumPosition - 1] < cumulativeEnergies[i][minumumPosition + 1])
                        minumumPosition = minumumPosition - 1;
                    else
                        minumumPosition = minumumPosition + 1;
                }
            }

            // std::cout << "Next postion: " << minumumPosition << std::endl;
        }

        for (int a = minumumPosition; a < maxColumn - 1; a++)
        {
            originalValues[i][a] = originalValues[i][a + 1];
        }
        // else if (i > 0 && i < maxRow - 1)
        // {
        //     if (initialPosition == 0) {
        //         middlePoint = initialPosition;
        //         rightPoint = middlePoint +1;

        //         if(cumulativeEnergyMap[i][middlePoint] <= cumulativeEnergyMap[i][rightPoint]) initialPosition = middlePoint;
        //         else initialPosition = rightPoint;
        //         directions[i] = initialPosition;

        //     }
        // }
    }
}

//used calculate the cumulative energies horizontally
void getCumHValues(int **energyArray, int **cumulativeEnergyMap, int maxColumn, int maxRow)
{
    int i;
    int lowestValue;
    int lowestValuePosition;

    //getting the cumulative energy map
    for (i = maxColumn - 1; i > -1; i--)
    {
        for (int a = 0; a < maxRow; a++)
        {

            /*
             row == max
             row < max, col = 0
             row < max, col = max
             row < max, 0 < col < max
             */

            if (i == maxColumn - 1)
            {
                cumulativeEnergyMap[a][i] = energyArray[a][i];
            }
            else if (i < maxRow - 1)
            {
                if (a == 0)
                {
                    lowestValue = std::min(cumulativeEnergyMap[a][i + 1], cumulativeEnergyMap[a + 1][i + 1]);
                    cumulativeEnergyMap[a][i] = lowestValue + energyArray[a][i];
                }
                else if (a == maxRow - 1)
                {
                    cumulativeEnergyMap[a][i] = std::min(cumulativeEnergyMap[a][i + 1], cumulativeEnergyMap[a - 1][i + 1]) + energyArray[a][i];
                }
                else
                {
                    cumulativeEnergyMap[a][i] = std::min(std::min(cumulativeEnergyMap[a][i + 1], cumulativeEnergyMap[a + 1][i + 1]), cumulativeEnergyMap[a - 1][i + 1]) + energyArray[a][i];
                    // std::cout << "No problem here\n";
                }
            }
        }
    }
}


//removing the vertical seams
void takeHSeam(int **originalValues, int **cumulativeEnergies, int maxRow, int maxColumn)
{

    int minumumPosition = 0;
    int minimumEnergy = cumulativeEnergies[minumumPosition][0];

    //finding the first minimum energy on first column
    for (int a = 0; a < maxRow; a++)
    {
        if (minimumEnergy >= cumulativeEnergies[a][0])
        {
            minumumPosition = a;
            minimumEnergy = cumulativeEnergies[minumumPosition][a];
        }
        // std::cout << "Finding lowest origin: " << minimumEnergy << " at: " << minumumPosition << std::endl;
    }

    //deleting the first min value
    for (int a = minumumPosition; a < maxRow - 1; a++)
    {
        originalValues[a][0] = originalValues[a + 1][0];
    }

    // findng rest of path
    for (int i = 1; i < maxColumn; i++)
    {

        // check if current row is between max and 1
        if (i <= maxColumn - 1)
        {
            //checking for left column edge case
            if (minumumPosition == 0)
            {
                if (cumulativeEnergies[minumumPosition][i] < cumulativeEnergies[minumumPosition + 1][i])
                    minumumPosition = minumumPosition;
                else
                    minumumPosition = minumumPosition + 1;
            }

            //checking for right column edge case
            else if (minumumPosition == maxRow - 1)
            {
                if (cumulativeEnergies[minumumPosition][i] < cumulativeEnergies[minumumPosition + -1][i])
                    minumumPosition = minumumPosition - 1;
            }

            //checkinf for middle cases
            else if (minumumPosition > 0 && minumumPosition < maxRow - 1)
            {

                if (cumulativeEnergies[minumumPosition][i] < cumulativeEnergies[minumumPosition + 1][i])
                {
                    if (cumulativeEnergies[minumumPosition][i] < cumulativeEnergies[minumumPosition + 1][i])
                        minumumPosition = minumumPosition;
                    else
                        minumumPosition = minumumPosition + 1;
                }
                else if (cumulativeEnergies[minumumPosition - 1][i] < cumulativeEnergies[minumumPosition][i])
                {
                    if (cumulativeEnergies[minumumPosition - 1][i] < cumulativeEnergies[minumumPosition + 1][i])
                        minumumPosition = minumumPosition - 1;
                    else
                        minumumPosition = minumumPosition + 1;
                }
            }

            // std::cout << "Next postion: " << minumumPosition << std::endl;
        }

        // deleting flagged pixel
        for (int a = minumumPosition; a < maxRow - 1; a++)
        {
            originalValues[a][i] = originalValues[a + 1][i];
        }
    }
}

//writing new image to file
void writeToFile(int **fileItems, int initialMaxRow, int initialMaxColumn, std::string fileName)
{
    std::ofstream newFile(fileName.substr(0, fileName.size() - 4) + "_processed.pgm");

    if (!newFile.is_open())
    {
        std::cout << "Can't open output file" << fileName << std::endl;
        return;
    }

    newFile << "P2\n"
            << "#carved by ameteku\n";
    newFile << initialMaxColumn << " " << initialMaxRow << std::endl << 255 << std::endl;

    //writes the contents of the array to a .pgm file for viewing
    for (int i = 0; i < initialMaxRow; i++)
    {
        for (int j = 0; j < initialMaxColumn; j++)
        {
            newFile << " " << fileItems[i][j];
        }
        //ending line
        newFile<< std::endl;
    }

    newFile.close();
}
