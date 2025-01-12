import xml.etree.ElementTree as et

# File paths
circuit_file = "v1.circ"
hex_file = "hexdata.hex"

# Read the hexadecimal data from Turjo.hex
with open(hex_file, "r") as hex_file:
    hex_data = hex_file.read().strip()

# Parse the Logisim circuit file
xmlTree = et.parse(circuit_file)
root = xmlTree.getroot()

# Update only the specified ROM with the given attributes
for element in root.findall("circuit"):
    for i in element.findall("comp"):
        # Target the specific ROM based on attributes
        if i.get("name") == "ROM" and i.get("loc") == "(400,520)" and i.get("lib") == "4":
            for j in i.findall("a"):
                if j.get("name") == "contents":  # Update the "contents" attribute
                    j.text = f"addr/data: 8 20 \n{hex_data}"

# Write the updated circuit back to the file
xmlTree.write(circuit_file, encoding='UTF-8', xml_declaration=True)
