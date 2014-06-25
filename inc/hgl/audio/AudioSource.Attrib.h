	friend class AudioPlayer;

private:

	void InitPrivate();

	AudioBuffer *Buffer;

protected:

	uint index;

	bool pause;

	bool loop;
	float pitch;
	float gain;
	float cone_gain;
	Vector3f position;
	Vector3f velocity;
	Vector3f direction;
	ReferenceValue distance;
	float rolloff_factor;
	ConeAngle angle;

protected:

	uint GetIndex(){return index;}

	double GetCurTime();

	int GetState();
	float GetMinGain();
	float GetMaxGain();

	bool GetLoop(){return loop;}

	float GetPitch(){return pitch;}
	float GetGain(){return gain;}
	float GetConeGain(){return cone_gain;}

	float GetRolloffFactor(){return rolloff_factor;}

	virtual void SetLoop(bool);
	void SetPitch(float);
	void SetGain(float);
	void SetConeGain(float);
	void SetRolloffFactor(float);
