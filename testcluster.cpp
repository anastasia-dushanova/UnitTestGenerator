#include "testcluster.h"

TestCluster::TestCluster()
{
    parser = new JSONparser();
}

TestCluster::TestCluster(const QString fileName) : fileName(fileName){

    parser = new JSONparser(fileName);
}

TestCluster::~TestCluster(){
    delete parser;
}

void TestCluster::makeClusters(){

    parser->parseJSONCluster();
}
