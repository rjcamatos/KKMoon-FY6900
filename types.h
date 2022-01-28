#ifndef TYPES_H
#define TYPES_H

enum Mode {
    READ,
    WRITE
};

enum Channel {
    CH1 = 'M',
    CH2 = 'F'
};

typedef struct s_channel {
    //read write
    double waveform;
    double frequency;
    double amplitude;
    double offset;
    double duty;
    double phase;
    double onoff;
} t_channel;

typedef struct s_modulation {
    //read write
    double trigger_mode;
    double trigger_source;
    double frequency_fsk;
    double pulse_amount;
    double rate_am;
    double frequency_offset_fm;
    double phase_offset_pm;
    //write
    double manualTriggerSource;
} t_modulation;

typedef struct s_sweep {
    //write
    double object;
    double start;
    double end;
    double time;
    double mode;
    double direction;
    double start_stop;
    double source;
} t_sweep;

typedef struct s_measurment {
    //user defined
    double type; // 0 == FREQUENCY, 1 == COUNTER
    //read
    double frequency;
    double counting_value;
    double counting_period;
    double positive_pulse_width;
    double negative_pulse_width;
    double duty_cycle;
    //write
    double coupling_mode;
    double reset_counter;
    double pause;
    //read write
    double gate_time;
} t_measurment;

typedef struct s_waveform {
    Channel channel;
    unsigned int slot;
    unsigned int waveform_index;
    double *linePoints;
    unsigned int linePointsSize;

} t_waveform;

#endif // TYPES_H
