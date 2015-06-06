################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GillespieGenerator.cpp \
../src/Gillespie_new.cpp 

OBJS += \
./src/GillespieGenerator.o \
./src/Gillespie_new.o 

CPP_DEPS += \
./src/GillespieGenerator.d \
./src/Gillespie_new.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__cplusplus=201402L -D__GXX_EXPERIMENTAL_CXX1Y__ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


