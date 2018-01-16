package org.usfirst.frc.team1234.robot.subsystems;

import org.usfirst.frc.team1234.robot.RobotMap;
import org.usfirst.frc.team1234.robot.commands.ElevatorWithJoystick;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.command.Subsystem;

/**
 *
 */
public class Elevator extends Subsystem {

	private WPI_TalonSRX m_leftMotor = RobotMap.leftElevatorMotor;
	private WPI_TalonSRX m_rightMotor = RobotMap.rightElevatorMotor;

	public void initDefaultCommand() {
		setDefaultCommand(new ElevatorWithJoystick());
	}

	public void set(double speed) {
		m_leftMotor.set(speed);
		m_rightMotor.set(speed);
	}

	public void stop() {
		m_leftMotor.stopMotor();
		m_rightMotor.stopMotor();
	}
}
