/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package org.usfirst.frc.team1234.robot.subsystems;

import org.usfirst.frc.team1234.robot.RobotMap;
import org.usfirst.frc.team1234.robot.commands.ArcadeDriveWithJoystick;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.SpeedController;
import edu.wpi.first.wpilibj.SpeedControllerGroup;
import edu.wpi.first.wpilibj.command.Subsystem;

/**
 * An example subsystem. You can replace me with your own Subsystem.
 */
public class DriveTrain extends Subsystem {
	public static boolean squaredInputs = true;
	private SpeedController m_leftMotor = new SpeedControllerGroup(RobotMap.frontLeftDriveTrainMotor,
			RobotMap.rearLeftDriveTrainMotor);
	private SpeedController m_rightMotor = new SpeedControllerGroup(RobotMap.frontRightDriveTrainMotor,
			RobotMap.rearRightDriveTrainMotor);

	
	public DriveTrain() {
		
		m_rightMotor.setInverted(true);
	}

	public void initDefaultCommand() {
		setDefaultCommand(new ArcadeDriveWithJoystick());
	}

	public void arcadeDrive(double speed, double curve) {
		
		m_leftMotor.set(speed + curve);
		m_rightMotor.set(speed - curve);
		 
	}

	public void arcadeDrive(Joystick joystick) {
		double speed = -joystick.getRawAxis(1) * 0.8;
		double curve = joystick.getRawAxis(0) * 0.6;

		
		
		m_leftMotor.set(speed + curve);
		m_rightMotor.set(speed - curve);
	}
}
