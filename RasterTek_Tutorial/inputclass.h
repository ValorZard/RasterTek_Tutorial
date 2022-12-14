#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

/////////////////////////////
// class name: InputClass //
///////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
private:
	bool m_keys[256]; // store state of each key in a keyboard array
};

#endif
