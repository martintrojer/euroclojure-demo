function webserver(request,res){
    local responseTable = http.urldecode(request.body);
    local parsedTable = http.jsondecode(responseTable.body);
    //server.log(parsedTable.id);
 
    //RETURN TRIGGER DS AND VALUE
    local trig = parsedTable.triggering_datastream.value.value;
    //return to cosm
    res.send(200, "OK");
    //print to log - could also send to device
    server.log(trig);
    
    device.send("level", trig);
}
//setup pieces - not part of function
server.log("access me via "+http.agenturl());
http.onrequest(webserver);

