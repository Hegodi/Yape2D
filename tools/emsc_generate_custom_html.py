import sys
import os
import glob
import re

key_title = "WEB_TITLE"
key_javascript_file = "JAVASCRIPT_FILE"
template_html = "tools/template.html"
build_path_emscripten = "build/emscripten/"

def generate_html(web_title, javascript_file):
    try:
        with open(template_html, 'r', encoding='utf-8') as file:
            content = file.read()
        
        content = content.replace(key_javascript_file, javascript_file)
        content = content.replace(key_title, web_title)
        
        output_file = build_path_emscripten + os.path.splitext(javascript_file)[0] + ".html"
        with open(output_file, 'w', encoding='utf-8') as file:
            file.write(content)
        
        print(f"New HTML generate and saved as {output_file}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":

    project_name = None

    with open("CMakeLists.txt", 'r', encoding='utf-8') as file:
        for line in file:
            match = re.search(r'project\((.*?)\)', line)
            if match:
                project_name = match.group(1)  # Return the content inside the parentheses

    generate_html("Hegodi - " + project_name, project_name + ".js")
