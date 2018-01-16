/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package org.usfirst.frc.team1234.robot;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
public class RobotMap {
	// all values temporary

	public static final int joystickPort = 0;
//	public static final Compressor c = new Compressor(18);

	// actuator mappings

	public static final WPI_TalonSRX frontLeftDriveTrainMotor = new WPI_TalonSRX(3);
	public static final WPI_TalonSRX rearLeftDriveTrainMotor = new WPI_TalonSRX(2);
	public static final WPI_TalonSRX frontRightDriveTrainMotor = new WPI_TalonSRX(17);
	public static final WPI_TalonSRX rearRightDriveTrainMotor = new WPI_TalonSRX(16);

	public static final WPI_TalonSRX leftElevatorMotor = new WPI_TalonSRX(5);
	public static final WPI_TalonSRX rightElevatorMotor = new WPI_TalonSRX(4);

	// public static final WPI_TalonSRX leftFeederMotor = new WPI_TalonSRX(6);
	// public static final WPI_TalonSRX rightFeederMotor = new WPI_TalonSRX(13);

	public static final WPI_TalonSRX leftShooterMotor = new WPI_TalonSRX(14);
	public static final WPI_TalonSRX rightShooterMotor = new WPI_TalonSRX(15);

	// public static final Solenoid shooterSolenoidForwardChannel = new Solenoid(0);
	// public static final Solenoid shooterSolenoidReverseChannel = new Solenoid(1);
	// public static final Solenoid shooterTiltChannel = new Solenoid(2);

	// sensor mappings

	// public static final Encoder leftDriveTrainEncoder = new Encoder(1, 2); //
	// Reassign
	// public static final Encoder rightDriveTrainEncoder = new Encoder(3, 4); //
	// Reassign
	// public static final Encoder elevatorEncoder = new Encoder(7, 8);

}
