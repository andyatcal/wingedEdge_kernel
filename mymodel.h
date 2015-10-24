#ifndef MYMODEL
#define MYMODEL

class MyModel{
public:
	float cube_hight;
	float cube_width;
	float cube_length;
	MyModel();
};

MyModel::MyModel() {
	cube_length = 1;
	cube_width = 1;
	cube_hight = 1;
}

#endif //MYMODEL