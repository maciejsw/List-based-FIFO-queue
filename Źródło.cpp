#include<iostream>
#include<list>
#include<string>

using namespace std;

struct Queue { //queue struct
    list<int>::iterator front; //iterators pointing to the front and end of queue
    list<int>::iterator end;
    int count = 0; //size of queue
};

void Push(list<int>* base, Queue* queue, int value) {
    list<int>::iterator last;
    list<int>::iterator first;

    if (!base->empty()) { //if list is not empty save first and last node of the list
        last = prev(base->end());
        first = base->begin();
    }

    if (base->empty()) { //if base is empty 
        base->push_front(value); //create the list
        queue->front = queue->end = base->begin(); //set the queue
        (queue->count)++;
    }
    else if (!(queue->count)) { //if there is no queue
        *last = value; //update value of the node
        last = prev(base->end()); //update last pointer
        (queue->front) = (queue->end) = last; //set queue
        (queue->count)++; 
    }
    else if (((int)base->size() == 1) || ((int)base->size() == queue->count)) { //if list has one element or the whole list is taken by queue
        base->insert(queue->end, value); //add new node to the list
        (queue->end) = prev(queue->end); //shift the end of the queue
        (queue->count)++;
    }
    else if ((queue->front != last) && (queue->end == first)) { //if there are any nodes behind the queue
        *last = value; //update value of the node
        last = prev(base->end()); //update last pointer
        queue->end = last; //end of the queue goes behind the front
        (queue->count)++;
    }
    else {
        *prev(queue->end) = value; //update the value of the node
        queue->end = prev(queue->end); //shift the end of the queue
        (queue->count)++;
    }
}

void Pop(list<int>* base, Queue* queue) {
    if (!(queue->count)) cout << "NULL" << endl; //if empty queue then NULL
    else {
        cout << *(queue->front) << endl;
        if (queue->front == base->begin()) queue->front = prev(base->end()); //if queue is "cycled" then cycle back xD
        else if (queue->count > 1) queue->front = prev(queue->front); //just shift the front
        (queue->count)--;
    }
}

void Print(list<int> base, bool direction) {
    if (base.empty()) { //if empty list then null
        cout << "NULL" << endl;
        return;
    }
    if (direction) base.reverse(); //if print backward then reverse the copy of the list
    for (list<int>::const_iterator i = base.begin(); i != base.end(); ++i) { //iterate through the list
        cout << *i << " ";
    }
    cout << endl;
}

void PrintQueue(list<int>* base, Queue* queue) {
    if (!(queue->count)) { //if queue is empty then NULL
        cout << "NULL" << endl;
        return;
    }
    list<int>::iterator first = base->begin();
    list<int>::const_iterator i = queue->front;
    while (i != queue->end) { //iterate through the queue
        cout << *i << " ";
        if (i == first) i = base->end();
        --i;
    }
    cout << *i;
    cout << endl;
}

bool CheckElement(list<int>* base, Queue* queue, list<int>::iterator* node) {
    list<int>::const_iterator i = queue->front;
    list<int>::iterator first = base->begin();
    while (i != queue->end) { //iterate through the queue
        if (i == *node) return true; //check if node belongs to the queue
        if (i == first) i = base->end();
        --i;
    }
    if (i == *node) return true;
    return false;
}

void DelBeg(list<int>* base, Queue* queue) {
    list<int>::iterator beg = base->begin();
    if (base->empty()) cout << "NULL" << endl; //if empty list then NULL
    else if (queue->count == 1 && CheckElement(base, queue, &beg)) { //if deleted node is the only in queue just reduce size
        queue->count = 0;
    }
    else if (CheckElement(base, queue, &beg)) { //if deleted node belongs to the queue
        if (queue->end == base->begin()) queue->end = next(queue->end); //if its beginning just shift
        else if (queue->front == base->begin()) queue->front = prev(base->end()); //if its end reverse the cycling
        (queue->count)--; 
    }
    base->pop_front();

}

void DelEnd(list <int>* base, Queue* queue) {
    list<int>::iterator beg = prev(base->end());
    if (base->empty()) cout << "NULL" << endl; //if empty list then NULL
    else if (queue->count == 1 && CheckElement(base, queue, &beg)) { //if deleted node is the only in queue just reduce size
        queue->count = 0;
    }
    else if (CheckElement(base, queue, &beg)) { //if deleted node belongs to the queue
        if (queue->end == prev(base->end())) queue->end = base->begin(); //if its beginning reverse the cycling
        else if (queue->front == prev(base->end())) queue->front = prev(queue->front); //if its end just shift
        (queue->count)--;
    }
    base->pop_back();

}

void GarbageSoft(list<int>* base, Queue* queue) {
    for (list<int>::iterator i = base->begin(); i != base->end(); ++i) { //iterate through the list
        if (!CheckElement(base, queue, &i)) {  //if node is not in the queue
            *i = 0; //clear it
        }
    }
}

void GarbageHard(list<int>* base, Queue* queue) {
    list<int>::iterator i = base->begin();
    while (i != base->end()) { //iterate through the list
        if (!CheckElement(base, queue, &i)) { //if node is not in the queue
            i = base->erase(i); //erase it
        }
        else ++i;
    }
}

void AddBeg(list<int>* base, Queue* queue, int value) {
    base->push_front(value);
    if (queue->count) {
        list<int>::iterator beg = base->begin();
        if (CheckElement(base, queue, &beg)) (queue->count)++; //if node is in the queue then increment the size
    }
}

void AddEnd(list<int>* base, Queue* queue, int value) {
    base->push_back(value);
    if (queue->count) {
        list<int>::iterator end = prev(base->end());
        if (CheckElement(base, queue, &end)) (queue->count)++; //if node is in the queue then increment the size
    }
}

enum commands {
    ADD_BEG,
    ADD_END,
    DEL_BEG,
    DEL_END,
    PRINT_FORWARD,
    PRINT_BACKWARD,
    SIZE,
    PUSH,
    POP,
    PRINT_QUEUE,
    COUNT,
    GARBAGE_SOFT,
    GARBAGE_HARD,
    UNKNOWN
};

commands FindCommand(string* s) {
    if (*s == "ADD_BEG") return ADD_BEG;
    if (*s == "ADD_END") return ADD_END;
    if (*s == "DEL_BEG") return DEL_BEG;
    if (*s == "DEL_END") return DEL_END;
    if (*s == "PRINT_FORWARD") return PRINT_FORWARD;
    if (*s == "PRINT_BACKWARD") return PRINT_BACKWARD;
    if (*s == "SIZE") return SIZE;
    if (*s == "PUSH") return PUSH;
    if (*s == "POP") return POP;
    if (*s == "PRINT_QUEUE") return PRINT_QUEUE;
    if (*s == "COUNT") return COUNT;
    if (*s == "GARBAGE_SOFT") return GARBAGE_SOFT;
    if (*s == "GARBAGE_HARD") return GARBAGE_HARD;
    return UNKNOWN;
}

int main() {

    ios_base::sync_with_stdio(0);
    cin.tie(0);

    string command;
    list<int> base;
    Queue queue;
    int value;

    while (cin >> command) {
        switch (FindCommand(&command)) {
        case ADD_BEG:
            cin >> value;
            AddBeg(&base, &queue, value);
            break;
        case ADD_END:
            cin >> value;
            AddEnd(&base, &queue, value);
            break;
        case DEL_BEG:
            DelBeg(&base, &queue);
            break;
        case DEL_END:
            DelEnd(&base, &queue);
            break;
        case PRINT_FORWARD:
            Print(base, 0);
            break;
        case PRINT_BACKWARD:
            Print(base, 1);
            break;
        case SIZE:
            cout << base.size() << endl;
            break;
        case PUSH:
            cin >> value;
            Push(&base, &queue, value);
            break;
        case POP:
            Pop(&base, &queue);
            break;
        case PRINT_QUEUE:
            PrintQueue(&base, &queue);
            break;
        case COUNT:
            cout << queue.count << endl;
            break;
        case GARBAGE_SOFT:
            GarbageSoft(&base, &queue);
            break;
        case GARBAGE_HARD:
            GarbageHard(&base, &queue);
            break;
        default:
            cout << "UNKNOWN COMMAND" << endl;
            break;
        }
    }
    return 0;
}