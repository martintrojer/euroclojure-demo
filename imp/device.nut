local index = 0;
local masterUART = array(100);      //serial buffer
 
//these should be changed to non character values
local startbit = 124;   //which is |
local endbit = 126;     //which is ~
 
//uart initialization
function startUART()
{
    hardware.configure(UART_57);
    hardware.uart57.configure(19200, 8, PARITY_NONE, 1, NO_CTSRTS);     //baud:19200, dataBits:8, parity, stopbit
}
 
agent.on("level", function(value) {      //receive data from agent
    //server.log("agent is on");
    //server.log(value);                //uncomment to print the data first
    
    hardware.uart57.write("X" + value + "Q");        //write data to UART
});
 
imp.configure("Hammer Banner", [], []);     //standard imp configure statement
startUART();    //setup uart
//checkUART();    //begin uart polling

