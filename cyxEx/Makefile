guardthread : GuardThread.cpp
	gcc GuardThread.cpp -o guardthread

threadsignal : ThreadSignal.cpp SemOP.cpp waitpid
	gcc ThreadSignal.cpp SemOP.cpp -o threadsignal

waitpid : WaitPID.cpp msgsend
	gcc WaitPID.cpp -o waitpid

msgsend : MessageSend.cpp msgrecv
	gcc MessageSend.cpp -o msgsend

msgrecv : MessageRecv.cpp semex
	gcc MessageRecv.cpp -o msgrecv

semex : SemOP.cpp SemEx.cpp copylast10k
	gcc SemOP.cpp SemEx.cpp -o semex

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
	rm -rf pipe fifo piperead pipewrite fatherandchild dest.txt copylast10k semex msgrecv msgsend waitpid threadsignal guardthread
	clear