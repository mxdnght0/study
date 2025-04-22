import sublime
import sublime_plugin


class ConvertNumberCommand(sublime_plugin.TextCommand):
    def run(self, edit):
        self.view.window().show_input_panel(
            "Enter the conversion in the format from_base -> to_base:",
            "",
            self.on_done,
            None,
            None
        )

    def on_done(self, user_input):
        try:
            from_base, to_base = user_input.split("->")
            from_base = int(from_base.strip())
            to_base = int(to_base.strip())

            if from_base < 2 or to_base < 2 or from_base > 10 or to_base > 10:
                self.show_popup("Base must be >= 2")
                return

            self.view.run_command("convert_number_edit", {
                "from_base": from_base,
                "to_base": to_base
            })

        except ValueError:
            self.show_popup("Please enter the conversion in the correct format [from_base -> to_base].")

    def show_popup(self, message):
        self.view.show_popup(
            '<body><p style="color: white;">{}</p></body>'.format(message),
            flags=sublime.HIDE_ON_MOUSE_MOVE_AWAY,
            location=self.view.sel()[0].begin(),
            max_width=400,
            max_height=100,
        )


class ConvertNumberEditCommand(sublime_plugin.TextCommand):
    def run(self, edit, from_base, to_base):
        for region in self.view.sel():
            if not region.empty():
                selected_text = self.view.substr(region).strip()

                try:
                    decimal_number = int(selected_text, from_base)
                    converted_number = self.convert_number(decimal_number, to_base)
                    self.view.replace(edit, region, converted_number)
                except ValueError:
                    self.show_popup(f"Invalid number '{selected_text}' for base {from_base}.")

    def convert_number(self, decimal_number, to_base):
        if decimal_number == 0:
            return "0"

        result = ""
        while decimal_number > 0:
            result = str(decimal_number % to_base) + result
            decimal_number //= to_base
        return result

    def show_popup(self, message):
        self.view.show_popup(
            '<body><p style="color: DeepPink;">{}</p></body>'.format(message),
            flags=sublime.HIDE_ON_MOUSE_MOVE_AWAY,
            location=self.view.sel()[0].begin(),
            max_width=400,
            max_height=100
        )
