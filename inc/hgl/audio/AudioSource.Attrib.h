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
	float ref_dist,max_dist;
    uint distance_model;
	float rolloff_factor;
	ConeAngle angle;

protected:

	uint GetIndex()const{return index;}

	double GetCurTime();

	int GetState();
	float GetMinGain();
	float GetMaxGain();

	const bool GetLoop()const{return loop;}

	const float GetPitch()const{return pitch;}
	const float GetGain()const{return gain;}
	const float GetConeGain()const{return cone_gain;}

	const uint GetDistanceModel()const{return distance_model;}
	const float GetRolloffFactor()const{return rolloff_factor;}

	const void GetDistance(double &rd,double &md)const
	{
        rd=ref_dist;
        md=max_dist;
    }

	virtual void SetLoop(bool);
	void SetPitch(float);
	void SetGain(float);
	void SetConeGain(float);
    void SetDistanceModel(uint);
	void SetRolloffFactor(float);
