#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <limits>
#include <queue>
#include <map>

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
int RSSintSlotDefault = std::numeric_limits<int>::max();
int IsNotimmdiete(string oprand){
    return oprand[0]=='F';
}
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
const int OperationCycle[6] = {2, 2, 10, 40, 2, 2};// OC[Opetation.ADD] = 2;

struct HardwareConfig
{
	int LoadRSsize;	 // number of load reservation stations
	int StoreRSsize; // number of store reservation stations
	int AddRSsize;	 // number of add reservation stations
	int MultRSsize;	 // number of multiply reservation stations
	int FRegSize;	 // number of fp registers
};

class RegisterResultStatuses;
class ReservationStations;
class CommonDataBus
{
public:
    CommonDataBus(){

    }
    void updateStatusBroadcast(RegisterResultStatuses &_registers, ReservationStations &_stations);
    struct DataItem {
        string toBeupdatedOprand1;
        string toBeupdatedOprand2;
        string toBeupdatedDes;
        int originStation;  // 数据来源的保留站标识
        // 其他可能的属性...
    };
    void append(string oprand1, string oprand2, string destination){
        DataItem temp = DataItem();
        temp.toBeupdatedOprand1 = oprand1;
        temp.toBeupdatedOprand2 = oprand2;
        temp.toBeupdatedDes = destination;
    }
private:

    std::vector<DataItem> dataItems;  // 存储数据项
    // 其他成员变量...
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
    int dataReady;
    RegisterResultStatus(){
        ReservationStationName="";
        dataReady=1;
    }
    RegisterResultStatus(string name, int dataReady){
        this->ReservationStationName = name;
        this->dataReady = dataReady;
    }
};

/*********************************** ↓↓↓ Todo: Implement by you ↓↓↓ ******************************************/
struct Instruction{
    string Opcode;
    string Destination;
    string Oprand1;
    string Oprand2;
    bool operator==(const Instruction& other) const {
        // 根据需要比较的属性进行比较
        return Opcode == other.Opcode&& Destination == other.Destination&& Oprand2 == other.Oprand2&& Oprand1 == other.Oprand1;
        // 如果还有其他需要比较的属性，可以在这里继续添加比较逻辑
    }
    //Instruction(string Opcode, string Destination, string Oprand1, string Oprand2):
    //Opcode(Opcode),Destination(Destination),Oprand1(Oprand1),Oprand2(Oprand2){}
};



class RegisterResultStatuses
{
public:
    RegisterResultStatus getOrDefaultInRS( const string& name, const RegisterResultStatus& defaultValue) {
        auto it = _registers.find(name);
        return it != _registers.end() ? it->second : defaultValue;
    }
	// ...
    RegisterResultStatuses(HardwareConfig hardwareConfig){
        this->hardwareConfig = hardwareConfig;
        for (int i = 0; i < hardwareConfig.FRegSize; ++i) {
            _registers.insert(std::make_pair("F"+std::to_string(i),RegisterResultStatus()));
            cout<<"F"+std::to_string(i)<<endl;
        }
    }

/*********************************** ↑↑↑ Todo: Implement by you ↑↑↑ ******************************************/
	/*
	Print all register result status. It is called by PrintRegisterResultStatus4Grade() for grading.
	If you don't want to write such a class, then make sure you call the same function and print the register
	result status in the same format.
	*/
    // plan to rewrite this fuc cuz I want to use map instead of vector for registers.
    string _printRegisterResultStatus() const {
        std::ostringstream result;
        for (const auto& pair : _registers) {
            result << pair.first << ": "; // 键（寄存器名称）
            result << pair.second.ReservationStationName << ", ";
            result << "dataRdy: " << (pair.second.dataReady ? "Y" : "N") << ", ";
            result << "\n";
        }
        return result.str();
    }
    void updateDatabus(CommonDataBus& cbd){


    }

    //迷惑的顺序???????似乎已经在RRS里实现了,你这两个类已经贴贴到负距离了.
   // void writeInstruction(Instruction tobeIssued){
       // toBeIssued = tobeIssued;
        //string destination = toBeIssued.Destination;
        //string oprand1 = toBeIssued.Oprand1;
        //string oprand2 = toBeIssued.Oprand2;
    //}
	//string _printRegisterResultStatus() const
	//{
		//std::ostringstream result;
		//for (int idx = 0; idx < _registers.size(); idx++)
		//{
		//	result << "F" + std::to_string(idx) << ": ";
		//	result << _registers[idx].ReservationStationName << ", ";
		//	result << "dataRdy: " << (_registers[idx].dataReady ? "Y" : "N") << ", ";
		//	result << "\n";
		//}
		//return result.str();
	//}
/*********************************** ↓↓↓ Todo: Implement by you ↓↓↓ ******************************************/
    void updateFromCDB(string registerName) {
       _registers[registerName].dataReady = 1;
       _registers[registerName].ReservationStationName = "";
    }
    void updateByName(string registerName, int dataRdy, string reservationStationName){
        _registers[registerName].dataReady = dataRdy;
        _registers[registerName].ReservationStationName = reservationStationName;
    }
    RegisterResultStatus findByName(string registerName){
        auto it = _registers.find(registerName);  // 使用 find 方法寻找键

        if (it != _registers.end()) {
            // 如果找到了键，更新对应的值
            return it->second;
        }
        else{
            return RegisterResultStatus();
        }
    }


private:
    //<register name, status> pair to be imp
    std::map<string,RegisterResultStatus> _registers;
	//vector<RegisterResultStatus> _registers;
    HardwareConfig hardwareConfig;
    //需要流操作,似乎不需要List,
    Instruction toBeIssued;
    RegisterResultStatus ready = RegisterResultStatus(dataReadyMSG,1);

    //vector<Instruction> instructionList;??这是在干啥
    void updateIfFound(string registerName){
        auto it = _registers.find(registerName);
        if (it != _registers.end()) {
            // 键被找到，执行相关操作

        }
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
    string destination;
    int issueCycle = -1;    // 发射周期
    int executeCycle = -1;  // 执行开始的周期
    int writeBackCycle = -1; // 写回周期

    ReservationStation(string name): name(name){
        isBusy = 0;
        opcode = "NULL";
        Vj = "NULL";
        Vk = "NULL";
        Qj = "NULL";
        Qk = "NULL";
        destination = RSStringSlotDefault;
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
        destination = RSStringSlotDefault;
    }
};

class ReservationStations
{
public:
    //什么sb抽象数据结构
    vector<std::pair<std::pair<Instruction,InstructionStatus>,ReservationStation>> IRtable;
    ReservationStations(HardwareConfig hardwareConfig,RegisterResultStatuses RRS,vector<std::pair<Instruction,InstructionStatus>>& InsTable):
    hardwareConfig(hardwareConfig),RRS(RRS){
        helperMap.insert(std::make_pair("ADD",Operation::ADD));
        helperMap.insert(std::make_pair("SUB",Operation::SUB));
        helperMap.insert(std::make_pair("MULT",Operation::MULT));
        helperMap.insert(std::make_pair("DIV",Operation::DIV));
        helperMap.insert(std::make_pair("LOAD",Operation::LOAD));
        helperMap.insert(std::make_pair("STORE",Operation::STORE));
        for (std::pair<Instruction,InstructionStatus>& csndm:InsTable) {
            IRtable.emplace_back(std::make_pair(csndm, ReservationStation()));
        }
        stationBuilder();
    };
    //耦合成抹了 迫真屎山代码
    void updateInsTable(vector<std::pair<Instruction,InstructionStatus>>& InsTable){
        for(int i = 0;i<InsTable.size();i++){
            InsTable[i].second = IRtable[i].first.second;
        }

    }
    void updateRS(int currentCycle){
        updateRSBasedOnRegisterStatus();
        for(ReservationStation& i: _stations){
            if(i.isBusy){
                int TableIndex = findIndexByReservationStation(i);
            if(i.Qj==RSStringSlotDefault&&i.Qk==RSStringSlotDefault){
                i.remainCycle--;
            }
            //这里ExcuteT
            if(i.remainCycle==0){
                IRtable[TableIndex].first.second.cycleExecuted = currentCycle;
                IRtable[TableIndex].first.second.cycleWriteResult = currentCycle+1;//偷个小懒问题不大.
                updateDatabus(cbd,i);
                cleanRS(i);
                //code to clean and boardcast
            }
            }
        }


    }
    void updateDatabus(CommonDataBus& cbd,ReservationStation i){
        cbd.append(i.Vj,i.Vk,i.destination);
    }
    //我要干什么:遍历一边所有保存站:如果vk和vj在Register中找到,则更新qj和qk
    void updateRSBasedOnRegisterStatus(){
        for(ReservationStation& i:_stations){
            if(i.isBusy) {
                RRS.updateByName(i.destination, 0, i.name);
                //如果source oprand的data不ready,更新,不然还是NULL'
                if(IsNotimmdiete(i.Vj)&&RRS.findByName(i.Vj).dataReady==0) {
                    i.Qj = RRS.findByName(i.Vj).ReservationStationName;
                }
                else{
                    i.Qj = RSStringSlotDefault;
                }
                if(IsNotimmdiete(i.Vj)&&RRS.findByName(i.Vj).dataReady==0) {
                    i.Qk = RRS.findByName(i.Vj).ReservationStationName;
                }
                else{
                    i.Qk = RSStringSlotDefault;
                }
            }
        }

    }
    int findIndexByReservationStation(ReservationStation& station) {
        for (size_t i = 0; i < IRtable.size(); ++i) {
            if (IRtable[i].second.name == station.name) { // 比较 ReservationStation
                return i;
            }
        }
        return -1; // 没有找到
    }
    int findIndexByInstruction(Instruction& instruction) {
        for (size_t i = 0; i < IRtable.size(); ++i) {
            if (IRtable[i].first.first == instruction) { // 比较 ReservationStation
                return i;
            }
        }
        return -1; // 没有找到
    }

    //to do这里值传递就好了//为了更新状态,这里要怎么做,这里issueT
    void writeRS(Instruction i, int currentCycle){
        int tableIndex = findIndexByInstruction(i);
        ReservationStation* correctLocation = findRSbyNameAndPointer(i.Opcode);
        //建立映射
        IRtable[tableIndex].second = *correctLocation;
        IRtable[tableIndex].first.second.cycleIssued = currentCycle;
        cout<<correctLocation->name<<endl;
        correctLocation->isBusy = 1;
        correctLocation->issueCycle = currentCycle;
        correctLocation->remainCycle = OperationCycle[helperMap[i.Opcode]];
        correctLocation->opcode = i.Opcode;
        correctLocation->Vj = i.Oprand1;
        correctLocation->Vk = i.Oprand2;
       //Qk,Qj如何设置???,Desstination用法?似乎和ResultRes有关.//这里就先载进去,Qk,Qj不管,
        correctLocation->destination =  i.Destination;

    }
    void printAll(){
        for(ReservationStation rs:_stations){
            cout<<rs.name<<rs.opcode<<rs.isBusy<<rs.Vk<<rs.destination<<rs.Vj<<rs.Qj<<rs.Vk<<rs.remainCycle<<endl;
        }
    }
	// ...
    void findAndSetForQJQK(string RGST) {
        for(ReservationStation &i:_stations){
            if(i.Vk==RGST){
                i.Vk = RSStringSlotDefault;
            }
            if(i.Vj==RGST){
                i.Vj = RSStringSlotDefault;
            }
        }

    }

private:
	vector<ReservationStation> _stations;
    RegisterResultStatuses RRS;
    HardwareConfig hardwareConfig;
    CommonDataBus cbd;
    int currentAddRSPointer = 0;
    int currentLoadRSPointer = 0;
    int currentMutiRSPointer = 0;
    int currentStoreRSPointer = 0;
    std::map<string, Operation> helperMap;
    //用来初始化中找到合适位置的方法,明明用map要方便一百万倍的.
    ReservationStation* findRSbyNameAndPointer(string Opcodename){
        ReservationStation toBeRuturnedRS;
        string RSslotByName;
        //get the correctpositionname by get a correctname
        if (Opcodename=="ADD"||Opcodename=="SUB"){
            RSslotByName = "ADD"+std::to_string(currentAddRSPointer);
            currentAddRSPointer++;

        }
        else if(Opcodename=="MULT"||Opcodename=="DIV"){
            RSslotByName = "MULT"+std::to_string(currentMutiRSPointer);
            currentMutiRSPointer++;
        }
        else if(Opcodename=="LOAD"){
            RSslotByName = Opcodename+std::to_string(currentLoadRSPointer);
            currentLoadRSPointer++;
        }
        else{
            RSslotByName = Opcodename+std::to_string(currentStoreRSPointer);
            currentStoreRSPointer++;
        }

        auto it = std::find_if(_stations.begin(), _stations.end(), [&RSslotByName](ReservationStation& item) {
            return item.name == RSslotByName;
        });
        return &(*it);
    }
    //update方法似乎无用,但留着先.我一定要有个操作可以减pointer,要不然要出事,howtodo.
    void updatePointer(Instruction i){
        if (i.Opcode=="ADD"||i.Opcode=="SUB"){
            currentAddRSPointer++;
        }
        else if(i.Opcode=="MULT"||i.Opcode=="DIV"){
            currentMutiRSPointer++;
        }
        else if(i.Opcode=="LOAD"){
            currentLoadRSPointer++;
        }
        else{
            currentStoreRSPointer++;
        }

    }//output, update pointer base on the input type;
    void stationBuilder(){
        for (int i = 0; i < hardwareConfig.AddRSsize; ++i) {
            _stations.push_back(ReservationStation("ADD"+std::to_string(i)));
        }
        for (int i = 0; i < hardwareConfig.MultRSsize; ++i) {
            _stations.push_back(ReservationStation("MULT"+std::to_string(i)));
        }
        for (int i = 0; i < hardwareConfig.LoadRSsize; ++i) {
            _stations.push_back(ReservationStation("LOAD"+std::to_string(i)));
        }
        for (int i = 0; i < hardwareConfig.StoreRSsize; ++i) {
            _stations.push_back(ReservationStation("STORE"+std::to_string(i)));
        }
    }

    void cleanRS(ReservationStation &toBeClean){

        int currentPointer = toBeClean.name.back() - '0';
            if (toBeClean.opcode=="ADD"||toBeClean.opcode=="SUB"){
                currentAddRSPointer = currentPointer;

            }
            else if(toBeClean.opcode=="MULT"||toBeClean.opcode=="DIV"){
                currentMutiRSPointer=currentPointer;
            }
            else if(toBeClean.opcode=="LOAD"){
                currentLoadRSPointer = currentPointer;
            }
            else{
                currentStoreRSPointer = currentPointer;
            }
            toBeClean.opcode = RSStringSlotDefault;
            toBeClean.isBusy = 0;
            toBeClean.Qj = RSStringSlotDefault;
            toBeClean.Qk = RSStringSlotDefault;
            toBeClean.Vj = RSStringSlotDefault;
            toBeClean.Vk = RSStringSlotDefault;
            toBeClean.remainCycle = RSSintSlotDefault;
    }


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
//这里 writeBackT.
void CommonDataBus::updateStatusBroadcast(RegisterResultStatuses &_registers, ReservationStations &_stations){
    for(DataItem i: dataItems){
        if(i.toBeupdatedDes.at(0)=='F'){
            _registers.updateFromCDB(i.toBeupdatedDes);
        }
        _stations.findAndSetForQJQK(i.toBeupdatedOprand1);
        _stations.findAndSetForQJQK(i.toBeupdatedOprand2);
        _stations.findAndSetForQJQK(i.toBeupdatedDes);

    }
}
//input 指针
void simulateTomasulo(vector<InstructionStatus> &instructionStatus, vector<std::pair<Instruction,InstructionStatus>> insTable,
                      RegisterResultStatuses &_register, ReservationStations &_rrs,CommonDataBus &CDB)
{
  int thiscycle = 1;
  int insTableIndex = 0;
	while (thiscycle < 100000000)
	{
        CDB.updateStatusBroadcast(_register,_rrs);
        //step 1 CDB updates//记录writeresult时间,更新RRS和RS.
        _rrs.updateRS(thiscycle);
        //step 2 Check RS and do the calculation
        _rrs.writeRS(insTable[insTableIndex].first,thiscycle);
        //step 3 for any available RS, load an instruction.//queue instead of vector.


		// Reservation Stations should be updated every cycle, and broadcast to Common Data Bus
		// ...

		// Issue new instruction in each cycle
		// ...
        _rrs.updateInsTable(insTable);
		// At the end of this cycle, we need this function to print all registers status for grading
		PrintRegisterResultStatus4Grade(outputtracename, _register, thiscycle);

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
    vector<Instruction> Instructions;
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
        Instructions.push_back(tobeAdded);

    }
    //for (auto it = instructionVector.begin(); it != instructionVector.end(); ++it) {
        //cout<<it->Opcode<<it->Destination<<it->Oprand1<<it->Oprand2<<endl;
        // 使用 *it 访问元素
    //}
    return Instructions;
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
    vector<InstructionStatus> instructionStatus;
    vector<Instruction> insQ = readInstructionFromFile(inputtracename);
    vector<std::pair<Instruction,InstructionStatus>> insTable;
    int thiscycle = 1; // start cycle: 1
    //建立空的
    for(int i = 0;i<insQ.size();i++){
        instructionStatus.push_back(InstructionStatus());
    }
    for(int i = 0;i<insQ.size();i++){
        insTable.emplace_back(insQ[i],instructionStatus[i]);
    }

    // Read instructions from a file (replace 'instructions.txt' with your file name)
	// ...
	// Initialize the register result status
	RegisterResultStatuses registerResultStatuses = RegisterResultStatuses(hardwareConfig);
	// ...
    ReservationStations RS = ReservationStations(hardwareConfig,registerResultStatuses,insTable);
    CommonDataBus CDB = CommonDataBus();

	// Initialize the instruction status table

	// ...
    simulateTomasulo(instructionStatus,insTable,registerResultStatuses,RS,CDB);
	// Simulate Tomasulo:
	// simulateTomasulo(registerResultStatus, instructionStatus, ...);

/*********************************** ↑↑↑ Todo: Implement by you ↑↑↑ ******************************************/

	// At the end of the program, print Instruction Status Table for grading
	PrintResult4Grade(outputtracename, instructionStatus);
	return 0;
}
