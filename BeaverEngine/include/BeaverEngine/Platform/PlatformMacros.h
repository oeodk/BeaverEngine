#pragma once

#if defined(__EMSCRIPTEN__)
#define GL_HEADER <GLES3/gl3.h>
#define PLATEFORM_MAIN_LOOP()																 \
struct LoopData																				 \
{																							 \
	bool* close_application;																 \
	std::chrono::steady_clock::time_point& old_time;										 \
	Game* game;																				 \
};																							 \
LoopData loop_data;																			 \
void main_loop() {																			 \
	static unsigned int frame = 0;															 \
	if (*loop_data.close_application) {														 \
		emscripten_cancel_main_loop();														 \
		return;																				 \
	}																						 \
	loop_data.game->loopBody(loop_data.old_time, frame, *loop_data.close_application);		 \
	frame++;																				 \
																							 \
}
#define MAIN_LOOP(old_time, frame, close_application, game, BODY) \
	loop_data.old_time = old_time;								  \
	loop_data.game = game;										  \
	loop_data.close_application = &close_application;			  \
	emscripten_set_main_loop(main_loop, fps_, true);			  


#else
#define GL_HEADER <glad/glad.h>
#define MAIN_LOOP(old_time, frame, close_application, game, BODY) \
	for (frame; !close_application; frame++)				      \
	{														      \
		BODY												      \
	}														

#define PLATEFORM_MAIN_LOOP ;
#endif

