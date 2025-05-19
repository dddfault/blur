
#pragma once

#include "common/rendering.h"
#include "common/rendering_frame.h"
#include "ui/ui.h"

namespace ui {
	class Container;
}

namespace gui::renderer {
	struct Notification {
		std::string id;
		std::chrono::steady_clock::time_point end_time;
		std::string text;
		ui::NotificationType type;
		std::optional<std::function<void()>> on_click_fn;
	};

	inline std::vector<Notification> notifications;
	inline const float NOTIFICATION_LENGTH = 4.f;
	inline std::mutex notification_mutex;

	inline bool just_added_sample_video = false;

	inline std::optional<Render> current_render_copy;

	enum class Screens : uint8_t {
		MAIN,
		CONFIG
	};

	inline Screens screen = Screens::MAIN;

	inline ui::Container main_container;
	inline ui::Container config_container;
	inline ui::Container config_preview_container;
	inline ui::Container option_information_container;
	inline ui::Container notification_container;
	inline ui::Container nav_container;

	namespace components {
		void render(
			ui::Container& container,
			Render& render,
			bool current,
			float delta_time,
			bool& is_progress_shown,
			float& bar_percent
		);

		void main_screen(ui::Container& container, float delta_time);

		namespace configs { // naming it configs to avoid conflict with common lol
			inline BlurSettings settings;
			inline BlurSettings current_global_settings;

			inline bool loaded_config = false;
			inline bool loading_config = false;

			inline bool interpolate_scale = true;
			inline float interpolated_fps_mult = 5.f;
			inline int interpolated_fps = 1200;

			inline bool pre_interpolate_scale = true;
			inline float pre_interpolated_fps_mult = 2.f;
			inline int pre_interpolated_fps = 360;

			inline std::vector<std::unique_ptr<FrameRender>> renders;
			inline std::mutex render_mutex;

			void set_interpolated_fps();
			void set_pre_interpolated_fps();

			void options(ui::Container& container, BlurSettings& settings);
			void preview(ui::Container& container, BlurSettings& settings);
			void option_information(ui::Container& container, BlurSettings& settings);

			void parse_interp();
			void save_config();
			void on_load();

			void screen(
				ui::Container& container,
				ui::Container& preview_container,
				ui::Container& option_information_container,
				float delta_time
			);
		}
	}

	bool redraw_window(bool rendered_last, bool force_render);

	void add_notification(
		const std::string& id,
		const std::string& text,
		ui::NotificationType type,
		const std::optional<std::function<void()>>& on_click = {},
		std::chrono::duration<float> duration = std::chrono::duration<float>(NOTIFICATION_LENGTH)
	);

	void add_notification(
		const std::string& text,
		ui::NotificationType type,
		const std::optional<std::function<void()>>& on_click = {},
		std::chrono::duration<float> duration = std::chrono::duration<float>(NOTIFICATION_LENGTH)
	);

	void render_notifications();

	void on_render_finished(Render* render, const RenderResult& result);
}
