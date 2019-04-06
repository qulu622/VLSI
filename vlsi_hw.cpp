#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

using namespace std;

string sourceName;
string pathDFS = "";

struct Node { // ��U�Ӹ`�I
	string name;
	int weight;
	bool visited;
};

struct Path { // ��U�Ӹ��|
	string name;
	int weight;
	string start;
	string end;
	bool visited;
};

struct Queue { // ��Queue,���u�O�}�C
	string name;
	int weight;
};

struct TmpPath { // ��ݭn�B�z�����| 
	string tmp1; // instance
	string tmp2; // node
	string name;
	int weight;
	string dir;
};

void Merge(int n1, Node * node, Queue * queue) { // ��node����Ƨ�s��queue��
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < n1; j++) {
			if (queue[i].name == node[j].name) {
				queue[i].weight = node[j].weight;
				break;
			}
		}
	}
}

void Sort(int index, int n1, Queue * queue) { // ��queue�Ƨ�
	for (int i = index + 1; i < n1; i++) {
		for (int j = i; j < n1; j++) {
			if (queue[i].weight > queue[j].weight) {
				string name = queue[i].name;
				int weight = queue[i].weight;
				queue[i].name = queue[j].name;
				queue[i].weight = queue[j].weight;
				queue[j].name = name;
				queue[j].weight = weight;
			}
		}
	}
}

void Compute_Dijkstra(int n1, int n2, Node * node, Path * path, Queue * queue) { // �p��O�_�n����v��
	int index;
	for (int i = 0; i < n1; i++) {
		index = i;
		string name = queue[index].name; // ����queue������ 
		for (int i = 0; i < n2; i++) {
			if (path[i].start == name) { // �_�I
				for (int j = 0; j < n1; j++) {
					if (node[j].name == path[i].end) { // ���I
						int weight;
						for (int k = 0; k < n1; k++) {
							if (node[k].name == name) {
								weight = node[k].weight; // �O���v��
								break;
							}
						}

						if (weight + path[i].weight < node[j].weight) // ��s�v��
							node[j].weight = weight + path[i].weight;
					}
				}
			}
		}

		Merge(n1, node, queue);
		Sort(index, n1, queue);
	}
}

void PrintResult(int n1, Node * node) { // �L���G


	cout << "DFS:" << endl;
	cout << pathDFS << endl << endl;

	cout << "Dijkstra: " << endl;
	for (int i = 0; i < n1; i++) {
		if (node[i].name != sourceName)
			cout << "From S to " << node[i].name << " = " << node[i].weight << endl;
	}
	
	cout << endl;
}

bool isNode(string tmp, int n1, Node * node) { // ���Rinput��,�ư��@�Ǥ��ݭn�����
	for (int i = 0; i < n1; i++) {
		if (tmp == node[i].name || tmp == "PAD" || tmp == "ai2s" || tmp == "i1s" || tmp == "dsr2s" ||
			tmp == "INSTANCE" || tmp == "//Source" || tmp == "//Destination" || tmp == "ENDCIRCUIT")
			return true;
	}
	return false;
}

void getData(string tmp, string & name, string & dir, int & weight) { // ���Rinput�� Name,Dir,Weight
	string get = "";

	for (int i = 0; i < tmp.length(); i++) {
		if (tmp[i] == '(') {
			name = get;
			get = "";
		}
		else if (tmp[i] == ')') {
			dir = get;
		}
		else if (tmp[i] == ',') {
			weight = atoi(get.c_str());
			get = "";
		}
		else
			get = get + tmp[i];
	}
}

void DFS(int n1, int n2, Node * node, Path * path,int index) {


	int new_index;
	for (int i = 0; i < n2; i++) {
		

		if (node[index].name == path[i].start && !path[i].visited ) {
			bool nodeUnVisited = true;
			for (int j = 0; j < n1; j++) {
				if (path[i].end == node[j].name && node[j].visited)
					nodeUnVisited = false;
			}

			if (nodeUnVisited) {
				path[i].visited = true;

				for (int j = 0; j < n1; j++) {
					if (path[i].end == node[j].name) {
						node[j].visited = true;
						pathDFS = pathDFS + " -> " + node[j].name;
						new_index = j;
					}
				}
				DFS(n1, n2, node, path, new_index);
			}
			
		}
	}
}

void Traversal(int n1, int n2, Node * node, Path * path) {
	for (int i = 0; i < n1; i++) {
		if (!node[i].visited) {
			node[i].visited = true;
			if (node[i].name == "S")
				pathDFS = pathDFS + node[i].name;
			else
				pathDFS = pathDFS + " -> " + node[i].name;
			DFS(n1, n2, node, path, i);
		}
	}
}

int main() {
	string inputname, outputname;

	cout << "�п�J�ɦW(+.txt): ";
	cin >> inputname;

	inputname = "input.txt";

	ifstream fin(inputname); // read


	if (!fin)
		cout << "fin fail" << endl;
	else { // fin + (cin) for Input, fout + (cout) for Output

		string tmp;
		int instance = 0; //  �p�⦳�X��instance
		int num = 0; // �p�⦳�X��path
		int count = 0; // Ū�ɭp���
		int max = 0; // �p����Nnode�̦h���X�Ӥlnode�A�}�}�C��.
		int nodeCount = 0; // �p��node��
		int allInput = 0; // ���`�@���X��input_token
		int pathCount = 0; // �p��path��

		fin >> tmp;
		allInput++;
		while (tmp != "ENDCIRCUIT") { // ��X���X��instance
			if (tmp == "INSTANCE")
				count++;
			fin >> tmp;
			allInput++;
		}
		instance = count;
		num = allInput - 5 - 3 * instance; 
		count = 0;

		fin.close(); //close file
		ifstream fin(inputname); // read

		Node * node = new Node[instance];
		Queue * queue = new Queue[instance];
		TmpPath * tmpPath = new TmpPath[num];

		// �A��Ū��
		if (!fin)
			cout << "fin fail" << endl;
		else {
			fin >> tmp;
			while (tmp != "ENDCIRCUIT") {
				if (tmp == "INSTANCE") {
					fin >> tmp;


					//���Ʃ�inode�Mqueue�� + ����l
					node[count].name = tmp;
					if (count == 0)
						sourceName = node[count].name;
					if (node[count].name == sourceName)
						node[count].weight = 0;
					else
						node[count].weight = 99999999;

					node[count].visited = false;
					queue[count].weight = node[count].weight;
					queue[count].name = node[count].name;

					nodeCount = 0;
					while (tmp != "INSTANCE" && tmp != "ENDCIRCUIT") {
						nodeCount++;
						fin >> tmp;

						if (!isNode(tmp, instance, node)) {
							tmpPath[pathCount].tmp1 = node[count].name;
							tmpPath[pathCount].tmp2 = tmp;
							pathCount++;
						}
					}

					count++;

					if (count == 1 || count == instance) // PAD�n�����⦸  ������
						nodeCount = nodeCount - 3;
					else
						nodeCount = nodeCount - 2;

					if (max < nodeCount)
						max = nodeCount;
				}
				else
					fin >> tmp;
			}

			int weight = 0;
			string name;
			string dir;
			for (int i = 0; i < num; i++)
				getData(tmpPath[i].tmp2, tmpPath[i].name, tmpPath[i].dir, tmpPath[i].weight);

			int pCount = 0;
			string * tmpAry = new string[num];
			for (int i = 0; i < num; i++) {
				bool isExist = false;
				for (int j = 0; j < num; j++) {
					if (tmpAry[j] == tmpPath[i].name)
						isExist = true;
				}

				if (!isExist) {
					tmpAry[pCount] = tmpPath[i].name;
					pCount++;
				}
			}

			Path * path = new Path[pCount];

			for (int i = 0; i < pCount; i++) { // ���Ʃ�ipath+����l
				path[i].name = tmpAry[i];
				path[i].visited = false;
			}

			for (int i = 0; i < num; i++) { //���Ʃ�ipath
				for (int j = 0; j < pCount; j++) {
					if (tmpPath[i].name == path[j].name) {
						path[j].weight = tmpPath[i].weight;
						if (tmpPath[i].dir == "i") {
							path[j].end = tmpPath[i].tmp1;
						}
						else { // o
							path[j].start = tmpPath[i].tmp1;
						}
					}
				}

			}

			
			for (int i = 0; i < pCount; i++) { // ��path�̷�end���Ƨ�
				for (int j = 0; j < pCount; j++) {
					string sEnd1 = "";
					string sEnd2 = "";

					for (int k = 1; k < path[i].end.length(); k++) 
						sEnd1 = sEnd1 + path[i].end[k];
					for (int k = 1; k < path[j].end.length(); k++) 
						sEnd2 = sEnd2 + path[j].end[k];

					int end1 = atoi(sEnd1.c_str());
					int end2 = atoi(sEnd2.c_str());

					if (end1 < end2) {
						Path tmpP ;
						tmpP.start = path[i].start;
						tmpP.end = path[i].end;
						tmpP.name = path[i].name;
						tmpP.weight = path[i].weight;
						// ���X���ΰ� �]���٨S�}�l�� ���Ofalse

						path[i].start = path[j].start;
						path[i].end = path[j].end;
						path[i].name = path[j].name;
						path[i].weight = path[j].weight;

						path[j].start = tmpP.start;
						path[j].end = tmpP.end;
						path[j].name = tmpP.name;
						path[j].weight = tmpP.weight;

					}
				}
			}

			Traversal(instance, pCount, node, path);
			Compute_Dijkstra(instance, pCount, node, path, queue);
			PrintResult(instance, node);

		}
	}

	fin.close(); //close file
	system("pause");
	return 0;
}