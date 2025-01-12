import subprocess
import xml.etree.ElementTree as et

# Paths to the files
circuit_file = "MIPS.circ"
hex_file = "hexdata.hex"
cpp_program = "./INSTRUCTION_CONVERTOR"  # Path to your compiled C++ program

# Step 1: Run the C++ program to generate Turjo.hex
try:
    subprocess.run(cpp_program, check=True)
    print(f"{hex_file} generated successfully by the C++ program.")
except subprocess.CalledProcessError as e:
    print(f"Error running the C++ program: {e}")
    exit(1)

# Step 2: Read the generated hexadecimal data
try:
    with open(hex_file, "r") as hex_file_obj:
        hex_data = hex_file_obj.read().strip()
except FileNotFoundError:
    print(f"{hex_file} not found. Make sure the C++ program generated it correctly.")
    exit(1)

# Step 3: Update the ROM contents in the Logisim circuit file
try:
    xmlTree = et.parse(circuit_file)
    root = xmlTree.getroot()

    # Find and update the specific ROM
    for element in root.findall("circuit"):
        for i in element.findall("comp"):
            if i.get("name") == "ROM" and i.get("loc") == "(400,520)" and i.get("lib") == "4":
                for j in i.findall("a"):
                    if j.get("name") == "contents":
                        j.text = f"addr/data: 8 20 \n{hex_data}"
                        print(f"ROM contents updated in {circuit_file}.")

    # Write the updated circuit back to the file
    xmlTree.write(circuit_file, encoding='UTF-8', xml_declaration=True)
    print(f"{circuit_file} updated successfully.")
except Exception as e:
    print(f"Error processing the circuit file: {e}")

# print(hex_data)
