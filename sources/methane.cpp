/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------
#include "precomp.h"
#include "global.h"

#include "doc.h"
#include "ClanMikmod/setupmikmod.h"


bool GLOBAL_SoundEnable = true;

//------------------------------------------------------------------------------
// Keyboard stuff

class SetupMikMod;

class SuperMethaneBrothers : public clan::Application
{
public:
	SuperMethaneBrothers();
	bool update();
private:
	void check_pause();
	void on_button_press(const clan::InputEvent& key);
	void on_joystick_press(const clan::InputEvent& key);
	void on_joystick_axis(const clan::InputEvent& key,int joystick);
	void on_joystick_axis1(const clan::InputEvent& key);
	void on_joystick_axis2(const clan::InputEvent& key);
	void on_window_close();

	void init_game();
	void run_game();
	enum class ProgramState
	{
		init_game,
		run_game,
		quit
	};
	ProgramState m_ProgramState = ProgramState::init_game;

	std::vector<clan::InputDevice> m_Joysticks;
	std::vector<clan::Slot> m_JoySlots;
	std::vector<clan::Slot> m_AxisSlots;
	std::vector<JOYSTICK*> m_Players;
	int m_LastKey = 0;
	int m_Joystick = 0;
	double sensitivity_horizontal = 0.3;
	double sensitivity_vertical = 0.7;
	bool m_Pause = false;
	bool m_CanPause = true;

	clan::SoundOutput m_SoundOutput;
	std::shared_ptr<SetupMikMod> m_SetupMikmod;
	clan::DisplayWindow m_Window;
	clan::Slot m_SlotQuit;
	clan::Slot m_SlotInput;
	clan::Canvas m_Canvas;

	clan::Font m_Font;

	std::shared_ptr<CMethDoc> m_Game;


	int disable_scale_flag = 0;
	int full_screen_flag = 1;
	int on_options_screen = 1;
	clan::GameTime m_GameTime;
};
clan::ApplicationInstance<SuperMethaneBrothers> clanapp;
void SuperMethaneBrothers::check_pause(){
	if(m_Joystick >= 1){
		if(m_Joysticks[0].get_keycode(JB_LS)){
			if(m_CanPause){
				m_Pause = !m_Pause;
				m_CanPause = false;
			}
		}else	if(!m_CanPause){
			m_CanPause = true;
		}
	}
	if(m_Joystick == 2){
		if(m_Joysticks[1].get_keycode(JB_LS)){
			if(m_CanPause){
				m_Pause = !m_Pause;
				m_CanPause = false;
			}
		}else m_CanPause = true;
	}
}
void SuperMethaneBrothers::on_button_press(const clan::InputEvent &key)
{
	m_LastKey = key.id;
}
void SuperMethaneBrothers::on_joystick_press(const clan::InputEvent &key)
{
	m_LastKey = key.id;
	if(m_LastKey == 0)
		m_LastKey = clan::keycode_enter;
}
void SuperMethaneBrothers::on_joystick_axis(const clan::InputEvent &key,int joystick)
{
	if(key.id==JA_DY){
		double x = (m_Window.get_game_controllers()[joystick]).get_axis(key.id);
		if(std::abs(0-x)==1){
			if(x>0){
				if(m_Players[joystick]->stop_input==true){
					m_Players[joystick]->stop_input = false;
					if(m_Players[joystick]->last_char=='A')
						m_Players[joystick]->last_char = 'Z';
					else
						m_Players[joystick]->last_char--;
				}
			}
			else if(x<0){
				if(m_Players[joystick]->stop_input==true){
					m_Players[joystick]->stop_input = false;
					if(m_Players[joystick]->last_char=='Z')
						m_Players[joystick]->last_char = 'A';
					else
						m_Players[joystick]->last_char++;
				}
			}
			m_Players[joystick]->key = m_Players[joystick]->last_char;
		}
		else
			m_Players[joystick]->stop_input = true;
	}
}
void SuperMethaneBrothers::on_joystick_axis1(const clan::InputEvent &key)
{
	SuperMethaneBrothers::on_joystick_axis(key,0);
}

void SuperMethaneBrothers::on_joystick_axis2(const clan::InputEvent &key)
{
	SuperMethaneBrothers::on_joystick_axis(key,1);
}
void SuperMethaneBrothers::on_window_close()
{
	m_LastKey = clan::keycode_escape;
}

SuperMethaneBrothers::SuperMethaneBrothers()
{
	clan::OpenGLTarget::set_current();
}

bool SuperMethaneBrothers::update()
{
	switch (m_ProgramState)
	{
		case ProgramState::init_game:
			init_game();
			break;
		case ProgramState::run_game:
			run_game();
			break;
		case ProgramState::quit:

			// We have a suspect race condition on program exit. Unsure where the source is
			m_Game.reset();
			m_SoundOutput.stop_all();
			clan::System::sleep(125);
			return false;
		default:
			return false;
	}
	return true;
}

void SuperMethaneBrothers::init_game()
{
	m_SoundOutput = clan::SoundOutput(44100, 192);
	m_SetupMikmod = std::make_shared<SetupMikMod>();

	// Set the video mode
	clan::DisplayWindowDescription desc;
	desc.set_title("Super Methane Brothers");
	desc.set_size(clan::Size(SCR_WIDTH * 6, SCR_HEIGHT *4), true);
	//desc.set_size(clan::Size(SCR_WIDTH * 2, SCR_HEIGHT *2), true);
	desc.set_allow_resize(true);
	m_Window = clan::DisplayWindow(desc);
	m_Canvas = clan::Canvas(m_Window);
	m_Game = std::make_shared<CMethDoc>(m_Canvas);

	// Connect the Window close event
	m_SlotQuit = m_Window.sig_window_close().connect(this, &SuperMethaneBrothers::on_window_close);

	// Connect a keyboard handler to on_key_up()
	m_SlotInput = m_Window.get_keyboard().sig_key_down().connect(this, &SuperMethaneBrothers::on_button_press);
	m_Joystick = 0;
	m_Joysticks = m_Window.get_game_controllers();
	for (auto &elem : m_Joysticks){
		m_Joystick++;
		m_JoySlots.push_back(elem.sig_key_down().connect(this, &SuperMethaneBrothers::on_joystick_press));
		if(m_Joystick==1)
			m_AxisSlots.push_back(elem.sig_axis_move().connect(this, &SuperMethaneBrothers::on_joystick_axis1));
		else
			m_AxisSlots.push_back(elem.sig_axis_move().connect(this, &SuperMethaneBrothers::on_joystick_axis2));
	}
	m_Game->InitGame();
	m_Game->LoadScores();
	m_Game->StartGame();
	m_Players.push_back(&m_Game->m_GameTarget.m_Joy1);
	m_Players.push_back(&m_Game->m_GameTarget.m_Joy2);
	m_Players[0]->last_char = m_Players[1]->last_char = 'A';
	m_Players[0]->stop_input = m_Players[1]->stop_input = true;
	m_Players[0]->player = 1;
	m_Players[1]->player = 2;
	m_Font = clan::Font("tahoma", 32);

	m_LastKey = 0;

	m_ProgramState = ProgramState::run_game;
	m_GameTime = clan::GameTime(25, 25);

}

void SuperMethaneBrothers::run_game()
{
	m_GameTime.update();
	if (m_LastKey == clan::keycode_escape||m_LastKey == 9)
	{
		m_Game->SaveScores();
		m_ProgramState = ProgramState::quit;
		return;
	}

	if (m_LastKey){
		if (on_options_screen){
			on_options_screen = 0;	// Start the game
			m_LastKey = 0;
		}

		m_Players[0]->key = m_Players[1]->key = ':';	// Fake key press (required for high score table)
		if ((m_LastKey >= clan::keycode_a) && (m_LastKey <= clan::keycode_z)) m_Players[0]->key = m_Players[1]->key = m_LastKey - clan::keycode_a + 'A';
		if ((m_LastKey >= clan::keycode_0) && (m_LastKey <= clan::keycode_9)) m_Players[0]->key = m_Players[1]->key = m_LastKey - clan::keycode_0 + '0';
		if (m_LastKey == clan::keycode_space) m_Players[0]->key = m_Players[1]->key = ' ';
		if (m_LastKey == clan::keycode_enter) m_Players[0]->key = m_Players[1]->key = 10;
		if (m_LastKey == clan::keycode_tab){
			m_Game->m_GameTarget.m_Game.TogglePuffBlow();
		}
		m_LastKey = 0;
	}

	SuperMethaneBrothers::check_pause();
	if(m_Pause==false){
		for(int player = 0;player<m_Joysticks.size();player++){
			if(m_Joysticks[player].get_keycode(JB_L2)||m_Joysticks[player].get_keycode(JB_R2)) m_Players[player]->fire = 1;
			else m_Players[player]->fire = 0;

			double x = m_Joysticks[player].get_axis(JA_LX);
			double y = m_Joysticks[player].get_axis(JA_LY);

			double c = m_Joysticks[player].get_axis(JA_DX);
			if(std::abs(0-c)==1){
				m_Players[player]->char_move = c;
			}
			else{
				m_Players[player]->char_move = 0;
				m_Players[player]->next_char = true;
			}

			if(x> sensitivity_horizontal) m_Players[player]->right = 1;
			else m_Players[player]->right = 0;

			if(x<-sensitivity_horizontal) m_Players[player]->left = 1;
			else m_Players[player]->left = 0;

			if(y> sensitivity_vertical) m_Players[player]->down = 1;
			else m_Players[player]->down = 0;

			if(y<-sensitivity_vertical||m_Joysticks[player].get_keycode(JB_RB)) m_Players[player]->up = 1;
			else m_Players[player]->up = 0;

			//double z = m_Joysticks[player].get_axis(6);
			//if(z<-sensitivity_vertical) m_Players[player]->next_level = -1;
			//else if(z>sensitivity_vertical) m_Players[player]->next_level = 1;
			//else m_Players[player]->next_level = 0;
		}
		for(int player = m_Joysticks.size();player<2;player++){
			if ((m_Window.get_keyboard()).get_keycode(KB_LEFT(player))) m_Players[player]->left = 1; 	else m_Players[player]->left = 0;
			if ((m_Window.get_keyboard()).get_keycode(KB_RIGHT(player))) m_Players[player]->right = 1; else m_Players[player]->right = 0;
			if ((m_Window.get_keyboard()).get_keycode(KB_UP(player))) m_Players[player]->up = 1; 		else m_Players[player]->up = 0;
			if ((m_Window.get_keyboard()).get_keycode(KB_DOWN(player))) m_Players[player]->down = 1; 	else m_Players[player]->down = 0;
			if ((m_Window.get_keyboard()).get_keycode(KB_FIRE(player)))m_Players[player]->fire = 1;else	m_Players[player]->fire = 0;
		}

		//------------------------------------------------------------------------------
		// Do game main loop
		//------------------------------------------------------------------------------
		m_Canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));
		if (on_options_screen){
			m_Game->DisplayOptions(m_Canvas, m_Font);
		}
		else{
			m_Game->MainLoop();
		}
		//------------------------------------------------------------------------------
		// Output the graphics
		//------------------------------------------------------------------------------

		m_Window.flip(0);
	}

}
