#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <vcg/math/similarity.h>
#include <wrap/gui/view.h>
#include <wrap/gui/trackmode.h>
#include <list>
#include <map>

namespace vcg {

class Transform {
public: 
  Transform();
  Similarityf track;
  Similarityf local;
};

Transform interpolate(const Transform &a, const Transform &b, float t);

class Trackball: public Transform {
public:
  enum Button { BUTTON_LEFT = 1, BUTTON_MIDDLE = 2, BUTTON_RIGHT = 4, WHEEL = 8,
		            KEY_SHIFT = 16, KEY_CTRL = 32, KEY_ALT = 64, HANDLE = 128 };

  Trackball();
  void SetIdentity();
  void SetPosition(const Similarityf &local, int millisec = 0);
  void SetTransform(const Transform &transform, int miilisec = 0);

  //operating
  void GetView();
  void Apply();
  void Draw();
  void Reset();

  //interface
  void MouseDown(int x, int y, Button button);
	void MouseMove(int x, int y); 
	void MouseUp(int x, int y, Button button); 
	void MouseWheel(Button notch);
  void ButtonUp(Button button);
  void ButtonDown(Button button);

  //default sensitivity 1
  void SetSensitivity(float s);

  //spinning interface
  void SetSpinnable(bool on);
  bool IsSpinnable();  
  void SetSpinning(Quaternionf &spin);
  void StopSpinning();
  bool IsSpinning();  

  //interfaccia navigation:
  void Back();
  void Forward();
  void Home();
  void Store();
  void HistorySize(int lenght);

  //internals

  enum System { LOCAL, VIEW, SCREEN };
  
  enum Motion { NONE = 0, ROTATE = 1, ROTATE_DUMMY = 2,         //really makes sense only in VIEW system
		            ROTATE_X = 3, ROTATE_Y = 4, ROTATE_Z = 5,		    // Axis Constrained Rotation 
		            DRAG_X = 6,   DRAG_Y = 7,   DRAG_Z = 8,					// Drag constrained to an axis (trackball axis)
		            DRAG_XY = 9,  DRAG_YZ = 10,  DRAG_XZ = 11,  		// Drag constrained to a plane
		            SCALE = 12                                      //scale respect to center of trackball		            
  };  

	

  struct Action {
    System system;
    Motion motion;
    Action() {}
    Action(System s, Motion m): system(s), motion(m) {}
  };
  ///Find the current action ussing the current button
  void SetCurrentAction();

protected:
  View<float> camera;
  Similarityf view;            //Rotate LOCAL coordinate into VIEW coordinates

  int current_button;
  Action current_action;

  TrackMode *CurrentMode();
  std::map<int, Action> actions;

  Similarityf last_track;
  Similarityf last_view;
  int last_x, last_y;
  bool dragging;
  int button_mask;

  Quaternionf spin;
  bool spinnable;
  bool spinning;
  
  std::list<Transform> history;
  int history_size;

   
  Point3f ScreenOrigin();      //center of trackball in Screen coord   
  Point3f ModelOrigin();       //center of trackball in Model coord
  
  Matrix44f ScreenToModel();  //forse non serve.....
  Similarityf ModelToLocal();
  
  //Point3f ScreenToLocal(const Point3f &p);
  //Point3f LocalToScreen(const Point3f &p);  
};


}//namespace

#endif