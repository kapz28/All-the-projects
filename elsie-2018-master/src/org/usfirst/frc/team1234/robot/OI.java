/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package org.usfirst.frc.team1234.robot;

import org.usfirst.frc.team1234.robot.commands.SetShooterSpeed;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.buttons.Button;
import edu.wpi.first.wpilibj.buttons.JoystickButton;

/**
 * This class is the glue that binds the controls on the physical operator
 * interface to the commands and command groups that allow control of the robot.
 */
public class OI {
	private Joystick m_joystick = new Joystick(RobotMap.joystickPort);

	public OI() {
		Button intakeButton = new JoystickButton(m_joystick, 8);
		Button shootButton = new JoystickButton(m_joystick, 7);

		intakeButton.whileHeld(new SetShooterSpeed(0.5));
		shootButton.whileHeld(new SetShooterSpeed(-0.5));

		/*
		 * buttons required: - arcade drive
		 * 
		 * - elevator up: y 4 press - elevator down: a 2 press - rotate cube: x 1 hold -
		 * feeder: rt 8 hold - shooter: lt 7 hold - tilt piston: lb 5 hold - push
		 * piston: rb 6 hold
		 */
	}

	public Joystick getJoystick() {
		return m_joystick;
	}
}
