#ifndef MYMODEL
#define MYMODEL

class MyModel{
public:
	float cube_hight;
	float cube_width;
	float cube_length;
	Model();
};

Model::Model() {
	cube_length = 1;
	cube_width = 1;
	cube_hight = 1;
}

#endif //MYMODEL