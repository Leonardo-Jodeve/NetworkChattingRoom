semex : SemOP.cpp signalEx.cpp copylast10k
	gcc SemOP.cpp signalEx.cpp -o semex

copylast10k : CopyLast10K.cpp fatherandchild
	gcc CopyLast10K.cpp -o copylast10k

fatherandchild : FatherAndChild.cpp pipe
	gcc FatherAndChild.cpp -o fatherandchild

pipe : Pipe.cpp piperead
	gcc Pipe.cpp -o pipe

piperead : NamedPipeRead.cpp pipewrite
	gcc NamedPipeRead.cpp -o piperead

pipewrite : NamedPipeWrite.cpp
	gcc NamedPipeWrite.cpp -o pipewrite

clean : 
	rm -rf pipe fifo piperead pipewrite fatherandchild dest.txt copylast10k semex
	clear