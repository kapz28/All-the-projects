package org.usfirst.frc.team1234.robot.subsystems;

import org.usfirst.frc.team1234.robot.RobotMap;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.command.Subsystem;

/**
 * Subsystem for shooter
 */
public class Shooter extends Subsystem {

	private WPI_TalonSRX m_leftShooter = RobotMap.leftShooterMotor;
	private WPI_TalonSRX m_rightShooter = RobotMap.rightShooterMotor;

	public Shooter() {
	}

	// Put methods for controlling this subsystem
	// here. Call these from Commands.

	public void initDefaultCommand() {
	}

	public void set(double speed) {
		m_leftShooter.set(speed);
		m_rightShooter.set(-speed);
	}

}
