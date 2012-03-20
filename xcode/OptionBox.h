#ifndef CS248_OPTIONBOX_H
#define CS248_OPTIONBOX_H

class OptionBox : public GameObject{
public:
	enum Type{START, RESTART, QUIT};
	Type ty;
};

#endif