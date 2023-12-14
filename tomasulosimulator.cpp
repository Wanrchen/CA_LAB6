#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <limits>
#include <queue>

using std::cout;
using std::endl;
using std::string;
using std::vector;

string inputtracename = "trace.txt";
// remove the ".txt" and add ".out.txt" to the end as output name
string outputtracename = inputtracename.substr(0, inputtracename.length() - 4) + ".out.txt";
string hardwareconfigname = "config.txt";
string RSStringSlotDefault = "NULL";
string dataReadyMSG = "dataRdy";
enum Operation
{
	ADD,
	SUB,
	MULT,
	DIV,
	LOAD,
	STORE
};
// The execute cycle of each operation: ADD, SUB, MULT, DIV, LOAD, STORE
const int OperationCycle[6] = {2, 2, 10, 40, 2, 2};// OC[ADD] = 2;

struct HardwareConfig
{
	int LoadRSsize;	 // number of load reservation stations
	int StoreRSsize; // number of store reservation stations
	int AddRSsize;	 // number of add reservation stations
	int MultRSsize;	 // number of multiply reservation stations
	int FRegSize;	 // number of fp registers
};

// We use the following structure to record the time of each instruction
struct InstructionStatus
{
	int cycleIssued;
	int cycleExecuted; // execution completed
	int cycleWriteResult;
};

// Register Result Status structure
struct RegisterResultStatus
{
	string ReservationStationName;
	bool dataReady;
};

/*********************************** ↓↓↓ Todo: Implement by you ↓↓↓ ******************************************/
struct Instruction{
    string Opcode;
    string Destination;
    string Oprand1;
    string Oprand2;
    //Instruction(string Opcode, string Destination, string Oprand1, string Oprand2):
    //Opcode(Opcode),Destination(Destination),Oprand1(Oprand1),Oprand2(Oprand2){}
};

class RegisterResultStatuses
{
public:
	// ...

/*********************************** ↑↑↑ Todo: Implement by you ↑↑↑ ******************************************/
	/*
	Print all register result status. It is called by PrintRegisterResultStatus4Grade() for grading.
	If you don't want to write such a class, then make sure you call the same function and print the register
	result status in the same format.
	*/
	string _printRegisterResultStatus() const
	{
		std::ostringstream result;
		for (int idx = 0; idx < _registers.size(); idx++)
		{
			result << "F" + std::to_string(idx) << ": ";
			result << _registers[idx].ReservationStationName << ", ";
			result << "dataRdy: " << (_registers[idx].dataReady ? "Y" : "N") << ", ";
			result << "\n";
		}
		return result.str();
	}
/*********************************** ↓↓↓ Todo: Implement by you ↓↓↓ ******************************************/
private:
	vector<RegisterResultStatus> _registers;
    HardwareConfig hardwareConfig;
    void initializeRRS(){

    }
};

// Define your Reservation Station structure
struct ReservationStation
{
    int remainCycle;
    int isBusy;
    string name;
    string opcode;
    string Vj;
    string Vk;
    string Qj;
    string Qk;
	ReservationStation(string name): name(name){
        isBusy = 0;
        opcode = "NULL";
        Vj = "NULL";
        Vk = "NULL";
        Qj = "NULL";
        Qk = "NULL";
        remainCycle = std::numeric_limits<int>::max();
    }
    ReservationStation(){
        name = "NULL";
        isBusy = 0;
        opcode = "NULL";
        Vj = "NULL";
        Vk = "NULL";
        Qj = "NULL";
        Qk = "NULL";
    }
};
class ReservationStations
{
public:
    ReservationStations(vector<Instruction> instructionList,HardwareConfig hardwareConfig):
    instructionList(instructionList),hardwareConfig(hardwareConfig){
        initialize();
    };
    void updateRS(){

    }
    //to do
    void writeRS(Instruction i){



    }
	// ...
private:
	vector<ReservationStation> _stations;
    std::queue<Instruction> waitList;
    vector<Instruction> instructionList;
    HardwareConfig hardwareConfig;
    int addCounter = 0;
    int loadCounter = 0;
    int mutiCounter = 0;
    int storeCounter = 0;
    void stationBuilder(){
        for (int i = 0; i < hardwareConfig.AddRSsize; ++i) {
            _stations.push_back(ReservationStation("ADD"+std::to_string(i)));
        }
        for (int i = 0; i < hardwareConfig.MultRSsize; ++i) {
            _stations.push_back(ReservationStation("ADD"+std::to_string(i)));
        }
        for (int i = 0; i < hardwareConfig.LoadRSsize; ++i) {
            _stations.push_back(ReservationStation("ADD"+std::to_string(i)));
        }
        for (int i = 0; i < hardwareConfig.StoreRSsize; ++i) {
            _stations.push_back(ReservationStation("ADD"+std::to_string(i)));
        }
    }
    void initialize(){
        stationBuilder();
        for(Instruction i:instructionList){
            if(i.Opcode=="ADD"||i.Opcode=="SUB"){
                if(addCounter<=hardwareConfig.AddRSsize){
                    writeRS(i);
                    addCounter++;
                }
                else{
                    waitList.push(i);
                }

            }
            else if(i.Opcode=="MULT"||i.Opcode=="DIV"){
                if(mutiCounter<=hardwareConfig.MultRSsize){
                    writeRS(i);
                    mutiCounter++;
                }
                else{
                    waitList.push(i);
                }


            }
            else if(i.Opcode=="LOAD"){
                if(loadCounter<=hardwareConfig.LoadRSsize){
                    writeRS(i);
                    loadCounter++;
                }
                else{
                    waitList.push(i);
                }


            }
            else{
                if(storeCounter<=hardwareConfig.StoreRSsize){
                    writeRS(i);
                    storeCounter++;
                }
                else{
                    waitList.push(i);
                }


            }
        }
            //判断type, 看看满没满,不满写入,满了加入waitList.
        }
        // to-do
    void cleanRS(ReservationStation toBeClean){

    }
    void readInsFromWaitlistTop(){
        writeRS(waitList.front());
        waitList.pop();
    }

};

class CommonDataBus
{
public:
	// ...
};
void PrintRegisterResultStatus4Grade(const string &filename,
                                     const RegisterResultStatuses &registerResultStatus,
                                     const int thiscycle)
{
    if (thiscycle % 5 != 0)
        return;
    std::ofstream outfile(filename, std::ios_base::app); // append result to the end of file
    outfile << "Cycle " << thiscycle << ":\n";
    outfile << registerResultStatus._printRegisterResultStatus() << "\n";
    outfile.close();
}
void PrintResult4Grade(const string &filename, const vector<InstructionStatus> &instructionStatus)
{
    std::ofstream outfile(filename, std::ios_base::app); // append result to the end of file
    outfile << "Instruction Status:\n";
    for (int idx = 0; idx < instructionStatus.size(); idx++)
    {
        outfile << "Instr" << idx << ": ";
        outfile << "Issued: " << instructionStatus[idx].cycleIssued << ", ";
        outfile << "Completed: " << instructionStatus[idx].cycleExecuted << ", ";
        outfile << "Write Result: " << instructionStatus[idx].cycleWriteResult << ", ";
        outfile << "\n";
    }
    outfile.close();
}
// Function to simulate the Tomasulo algorithm
void simulateTomasulo()
{

	int thiscycle = 1; // start cycle: 1
	RegisterResultStatuses registerResultStatus;

	while (thiscycle < 100000000)
	{

		// Reservation Stations should be updated every cycle, and broadcast to Common Data Bus
		// ...

		// Issue new instruction in each cycle
		// ...

		// At the end of this cycle, we need this function to print all registers status for grading
		PrintRegisterResultStatus4Grade(outputtracename, registerResultStatus, thiscycle);

		++thiscycle;

		// The simulator should stop when all instructions are finished.
		// ...
	}
};

/*********************************** ↑↑↑ Todo: Implement by you ↑↑↑ ******************************************/

/*
print the instruction status, the reservation stations and the register result status
@param filename: output file name
@param instructionStatus: instruction status
*/


/*
print the register result status each 5 cycles
@param filename: output file name
@param registerResultStatus: register result status
@param thiscycle: current cycle
*/
vector<Instruction> readInstructionFromFile(string inputtracename){
    vector<Instruction> instructionVector;
    std::ifstream trace;
    trace.open(inputtracename);
    string line;

    while(std::getline(trace,line)){
        Instruction tobeAdded;
        std::istringstream iss(line);
        iss>>tobeAdded.Opcode;
        iss>>tobeAdded.Destination;
        iss>>tobeAdded.Oprand1;
        iss>>tobeAdded.Oprand2;
        instructionVector.push_back(tobeAdded);

    }
    //for (auto it = instructionVector.begin(); it != instructionVector.end(); ++it) {
        //cout<<it->Opcode<<it->Destination<<it->Oprand1<<it->Oprand2<<endl;
        // 使用 *it 访问元素
    //}
    return instructionVector;
}


int main(int argc, char **argv)
{
	if (argc > 1)
	{
		hardwareconfigname = argv[1];
		inputtracename = argv[2];
	}

	HardwareConfig hardwareConfig;
	std::ifstream config;
	config.open(hardwareconfigname);
	config >> hardwareConfig.LoadRSsize;  // number of load reservation stations
	config >> hardwareConfig.StoreRSsize; // number of store reservation stations
	config >> hardwareConfig.AddRSsize;	  // number of add reservation stations
	config >> hardwareConfig.MultRSsize;  // number of multiply reservation stations
	config >> hardwareConfig.FRegSize;	  // number of fp registers
	config.close();

/*********************************** ↓↓↓ Todo: Implement by you ↓↓↓ ******************************************/
    vector<Instruction> insList = readInstructionFromFile(inputtracename);
	// Read instructions from a file (replace 'instructions.txt' with your file name)
	// ...

	// Initialize the register result status
	// RegisterResultStatuses registerResultStatus();
	// ...

	// Initialize the instruction status table
	vector<InstructionStatus> instructionStatus;
	// ...

	// Simulate Tomasulo:
	// simulateTomasulo(registerResultStatus, instructionStatus, ...);

/*********************************** ↑↑↑ Todo: Implement by you ↑↑↑ ******************************************/

	// At the end of the program, print Instruction Status Table for grading
	PrintResult4Grade(outputtracename, instructionStatus);
	return 0;
}
