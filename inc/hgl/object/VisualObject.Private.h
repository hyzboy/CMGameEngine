	friend class ObjectControl;

protected:

	bool visible;
	bool topmost;
	bool bottommost;

	virtual const bool is_visual	()const{return visible;}
	virtual const bool is_proc_event()const{return(visible&&enabled);}

protected:

	bool GetVisible(){return visible;}
	bool GetTopMost(){return topmost;}
	bool GetBottomMost(){return bottommost;}

	virtual void SetVisible(bool);
	virtual void SetTopMost(bool);
	virtual void SetBottomMost(bool);
