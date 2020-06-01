#include <iostream>
#include <vector>
#include <string>

using namespace std;


class for_restore
{
public:
	int num_1, num_2;
	char ch_1, ch_2;
};

class string_node
{
public:
	string_node(string str, int num)
	{
		this->num = num;
		this->node_s = str;
	}
	int num;
	int char_index;
	string node_s;
};

class make_bwt
{

public:

	// 생성자. 
	make_bwt(string original_string);

	// fill the rest 생성 
	void make_fill_the_rest();


	// 정렬된 테이블을 만들기 위한 함수들, fill the rest를 정렬한다.
	void sort_ftr();
	void sort_ftr_part(int start, int end, int index);
	void chk_overlap(int start, int end, int index);


	// bwt 스트링 생성
	void construct_bwt_string();


	// 벡터내의 요소들을 정렬/
	void quick_sort(int left, int right, int index);
	int quick_sort_part(int left, int right, vector<string_node*>& vec, int index);
	void swap_vector(vector<string_node*>& a, int i, int j);

	// 문자열을 정렬.
	void quick_sort(int left, int right, string& str);
	int quick_sort_part(int left, int right, string& str);
	void swap_char_in_string(string& str, int i, int j);

	// bwt 테이블 생성 
	void make_bwt_table();

	void show_fill_and_sorted();
	void restore_from_bwt();

	void concat_strings();



private:
	string original_string;
	string string_dollar;
	vector<string_node*> sorted_vector;
	vector<string_node*> fill_the_rest;
	string bwt_string;
	for_restore* bwt_order;
	int string_size;
	string restored_string;

	friend class string_node;

};

// for_restore 의 멤버중 ch_2 가 벡터 내에 이미 존재하는지 확인, 발견시 인덱스 반환
int find_char_in_vector(vector<for_restore*> a, char b)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i]->ch_2 == b)
			return i;
	}
	return -1;
}


make_bwt::make_bwt(string original_string)
{
	this->original_string = original_string;
	this->string_dollar = original_string + "$";
	this->bwt_string = "";
	this->string_size = string_dollar.length();
}

//////////////////////////////////////////////////////////////////////////////////////////////////

// 벡터 요소 정렬용 퀵소트
int make_bwt::quick_sort_part(int left, int right, vector<string_node*>& vec, int index)
{
	char pivot, tmp;
	int low, high;

	low = left;
	high = right + 1;
	pivot = vec[left]->node_s[index];

	do
	{
		do
		{
			low++;
		} while (low <= right && vec[low]->node_s[index] < pivot);

		do
		{
			high--;
		} while (high >= left && vec[high]->node_s[index] > pivot);

		if (low < high)
		{
			swap_vector(vec, low, high);
		}

	} while (low < high);

	swap_vector(vec, left, high);

	return high;
}

void make_bwt::quick_sort(int left, int right, int index)
{
	if (left < right)
	{
		int q = make_bwt::quick_sort_part(left, right, sorted_vector, index);

		make_bwt::quick_sort(left, q - 1, index);
		make_bwt::quick_sort(q + 1, right, index);

	}
}

void make_bwt::swap_vector(vector<string_node*>& a, int i, int j)
{
	string_node* tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

// 스트링 정렬용 퀵소트
int make_bwt::quick_sort_part(int left, int right, string& str)
{
	char pivot, tmp;
	int low, high;

	low = left;
	high = right + 1;
	pivot = str[left];

	do
	{
		do
		{
			low++;
		} while (low <= right && str[low]< pivot);

		do
		{
			high--;
		} while (high >= left && str[high] > pivot);

		if (low < high)
		{
			
			swap_char_in_string(str, low, high);
		}

	} while (low < high);

	swap_char_in_string(str, left, high);

	return high;
}

void make_bwt::quick_sort(int left, int right, string & str)
{
	if (left < right)
	{
		int q = make_bwt::quick_sort_part(left, right, str);

		make_bwt::quick_sort(left, q - 1, str);
		make_bwt::quick_sort(q + 1, right, str);

	}
}

void make_bwt::swap_char_in_string(string& str, int i, int j)
{
	char tmp = str[i];
	str[i] = str[j];
	str[j] = tmp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////


// fill the rest 를 만드는 함수. 
void make_bwt::make_fill_the_rest()
{
	string tmp = string_dollar;
	char tmp_c;

	for (int i = 0; i < string_size; i++)
	{
		string_node* node = new string_node(tmp, i);
		fill_the_rest.push_back(node);
		tmp_c = tmp[0];
		tmp.erase(0, 1);
		tmp = tmp+ tmp_c;
	}

	sorted_vector = fill_the_rest;
}


// 중복 되는 문자 존재시 중복되는 문자들 끼리 그 다음 문자들의 순서를 비교하여 정렬함
void make_bwt::chk_overlap(int start, int end, int index)
{
	int tmp_e = -1;
	for (int i = start; i <= end; i++)
	{
	
		if ( i+1 < string_size && sorted_vector[i]->node_s[index] == sorted_vector[i + 1]->node_s[index])
		{

			int j = i+1;

			do
			{
				j++;
			}while (j <= end && (sorted_vector[i]->node_s[index] == sorted_vector[j]->node_s[index]));

			make_bwt::sort_ftr_part(i, j -1 , index + 1);
			tmp_e = j-1;

		}
		if (tmp_e != -1)
		{
			i = tmp_e;
			tmp_e = -1;
		}
		
	}	
}

void make_bwt::sort_ftr()
{
	make_bwt::sort_ftr_part(0, string_size - 1, 0);
	int tmp_e = 0;

	sorted_vector[0]->char_index = 0;

	for (int i = 0; i < string_size - 1; i++)
	{
		if (sorted_vector[i]->node_s[0] == sorted_vector[i + 1]->node_s[0])
			sorted_vector[i + 1]->char_index = sorted_vector[i]->char_index + 1;
		else
			sorted_vector[i + 1]->char_index = 0;
	}

	make_bwt::construct_bwt_string();
}

void make_bwt::sort_ftr_part(int start, int end, int index)
{
	make_bwt::quick_sort(start, end  , index);
	make_bwt::chk_overlap(start, end , index);
}

void make_bwt::construct_bwt_string()
{
	for (int i = 0; i < string_size; i++)
	{
		bwt_string = bwt_string + sorted_vector[i]->node_s[sorted_vector.size() - 1];
	}
}

void make_bwt::show_fill_and_sorted()
{

	make_bwt::sort_ftr();

	cout << "Fill The Rest :: \n\n";
	for (int i = fill_the_rest.size() -1; i >= 0; i--)
	{
		cout << fill_the_rest[i]->num << "\t" << fill_the_rest[i]->node_s << endl;
	}

	cout << "\n\n Sorted Vector :: \n\n";
	for (int i = 0; i <= fill_the_rest.size() - 1; i++)
	{
		cout << sorted_vector[i]->num << "\t" << sorted_vector[i]->node_s << endl;
	}

	cout << "\n\nconstructed bwt string :: " << bwt_string << "\n\n";

	make_bwt::restore_from_bwt();
}

void make_bwt::make_bwt_table()
{
	string in_order = bwt_string;

	make_bwt::quick_sort(0, string_size - 1, in_order);

	vector<for_restore*> temp;
	bwt_order = new for_restore[string_size];


	bwt_order[0].ch_1 = in_order[0];
	bwt_order[0].num_1 = 0;

	for (int i = 0; i < string_size - 1; i++)
	{
		if (in_order[i] == in_order[i + 1])
		{
			bwt_order[i + 1].ch_1 = bwt_order[i].ch_1;
			bwt_order[i + 1].num_1 = bwt_order[i].num_1 + 1;
		}
		else
		{
			bwt_order[i + 1].ch_1 = in_order[i + 1];
			bwt_order[i + 1].num_1 = 0;
		}
	}

	for (int i = 0; i < string_size; i++)
	{
		int index = find_char_in_vector(temp, bwt_string[i]);
		if (index == -1)
		{
			for_restore* new_node = new for_restore();
			temp.push_back(new_node);
			temp[temp.size() - 1]->ch_2 = bwt_string[i];
			temp[temp.size() - 1]->num_2 = 0;
			bwt_order[i].ch_2 = bwt_string[i];
			bwt_order[i].num_2 = 0;
		}
		else
		{
			int j = temp[index]->num_2 + 1;
			bwt_order[i].ch_2 = bwt_string[i];
			bwt_order[i].num_2 = j;
			temp[index]->num_2 = j;
		}
	}

}

void make_bwt::restore_from_bwt()
{
	make_bwt::make_bwt_table();
	
	for (int i = 0; i < string_size; i++)
		cout << bwt_order[i].ch_1<< bwt_order[i].num_1<<"\t"
		<< bwt_order[i].ch_2<< bwt_order[i].num_2<< endl;

	concat_strings();

	cout << "restored string :: " <<restored_string << endl;
	
}

void make_bwt::concat_strings()
{
	int i = 0;
	int cnt = 0;
	restored_string = "$";
	while(true)
	{
		for (int j = 0; j < string_size; j++)
		{
			if (bwt_order[i].ch_2 == bwt_order[j].ch_1
				&& bwt_order[i].num_2 == bwt_order[j].num_1)
			{
				restored_string = bwt_order[i].ch_2 + restored_string;
				cnt++;
				i = j;
				break;
			}
		}
		if (cnt == string_size -1)
			break;
	}
}

int main()
{
	make_bwt* aa = new make_bwt("kimdohun");

	aa->make_fill_the_rest();
	aa->show_fill_and_sorted();

}
