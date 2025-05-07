#ifndef SETTINGS_CLASS_H
#define SETTINGS_CLASS_H

enum class Mode{NORMAL, CPU_MULTITHREAD, GPU_ACCELERATION, ULTRA};
enum class TimeMode{REAL_TIME, SIM_TIME};
enum class State{RUNNING, STOPPED};

class Settings{
    public:
        Settings();
        ~Settings();

        static Mode mode;
        static TimeMode time_mode;
        static State state;
    private:
};

#endif
