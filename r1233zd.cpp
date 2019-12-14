/**********************
 Tony Scarcia
 Written Dec 13, 2019
 **********************/
 /*
  * r1233zd.c
  * console program to calculte saturation data for refrigerant r1233zd from an external file
  * Tony Scarcia
  *
  */

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

/* define the Size of the Data Set to be an array of double precision floating point numbers*/
constexpr auto SIZE = (47);

// this is the temperature to pressure conversion algorithm
// it uses a simmple 3rd degree polynomial to approximate the saturation curve
double sat_temp_to_press(double t_in){
	double p_out;
    if (t_in <= 62.33) {
        p_out = 4.852e-6 * pow(t_in, 3) + 0.001076 * pow(t_in, 2) + 0.08865 * t_in - 11.78;
    }
    if (t_in > 62.33) {
        p_out = 1.647e-5 * pow(t_in, 3) - 0.00194 * pow(t_in, 2) + 0.4343 * t_in - 25.75;
    }
	return (p_out);
}
int main(void) {
    // declare and initialize variables
    int k, loop = 1;
	double temp[SIZE], press[SIZE], t_data, p_data, p_value, t_value, t_trial, p_trial;
    ifstream inFile;
	
    /* read r1233zd saturation data file */
    inFile.open("r1233zd.txt");
    if (!inFile) {
		cout << "Error while opening the file 'r1233zd.txt'";
		exit(1); //terminate with error if the file is not found
	}
	k = 0;
	while (inFile >> t_data >> p_data) {
        temp[k] = t_data;
		press[k] = p_data;
	    k++;
    }
    inFile.close();
    
    // test the algorithm repeatedly asking the user for input 
    while (loop) {
        // calculate and display the pressure based on the input temperature
        printf("Enter Saturation Temperature [degF]: ");
        cin >> t_value;
        p_value = sat_temp_to_press(t_value);
        cout << std::fixed << setw(7) << setprecision(3) << p_value << " PSIg" << endl;
    
        // calculate and display the temperature based on the input pressure
        p_value = 10;
        p_trial = 20;
        cout << "Enter Saturation Pressure [PSIg]: ";
        cin >> p_value;
        p_value = p_value * 1.0;

        // initial guess midway through the temperature range to reduce the iteration delay
        t_trial = 50;
        
        // check the error and re-calculate if necessary
        while (abs(p_value - p_trial) > 0.001) {
        
            // call the function to test the guess
            p_trial = sat_temp_to_press(t_trial);
            
            // increase the temperature guess if the resulting pressure is less than calculated pressure value
            if (p_trial < p_value) {
                t_trial = t_trial + 0.001;
            }
            // decrease the temperature guess if the resulting pressure is greater than calculated pressure value
            if (p_trial > p_value) {
                t_trial = t_trial - 0.001;
            }
        }
        cout << std::fixed << setw(7) << setprecision(3) << t_trial << " F" << endl;
    }
	/*
    // This is only to print the values of the two arrays for checking purposes
    // print the temperature array
	cout << endl;
	cout << "Temperature Array ***" << endl;
    int i = 0;
    while (i < SIZE){
        cout << temp[i] << endl;
        i++;
    }
    // print the pressure array
	cout << endl;
    cout << "** Pressure Array ***" << endl;
    i = 0;
    while (i < SIZE){
        cout << press[i] << endl;
        i++;
    }
	*/
}
