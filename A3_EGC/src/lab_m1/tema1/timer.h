#pragma once


namespace timer
{
    // Translate matrix
    class Timer
    {
    public:
        float time_left;
        float duration;
        Timer(): duration(0) {}

        Timer(float duration)
        {
            time_left=duration;
            this->duration=duration;
        }

        float progress(float deltaTime)
        {
            if(duration==0)
                return -1;
            float passedTime=(time_left<deltaTime)?time_left:deltaTime;
            time_left-=passedTime;
            return passedTime;
        }
        bool isDone()
        {
            return duration!=0 && time_left==0;
        }

		inline float persantage_left()
		{
			return time_left/duration;
		}

		inline float persantage_done()
		{
			return 1.f-persantage_left();
		}
    };
}   // namespace transform2D
