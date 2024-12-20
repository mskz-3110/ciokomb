import yaml
import os
import sys

saveConfig = {
  "Key": "Value",
  "Japanese": 'あいうえお',
  "Empty": '',
  "Escape": "1\\\\2\\\'3\\\"4\\t5\\n6",
}

filePath = """{}/test_string.yaml""".format(os.path.dirname(__file__))

if os.path.isfile(filePath) is not True:
  with open(filePath, "w", encoding = "utf-8", newline = "\n") as file:
    yaml.dump(saveConfig, file, sort_keys = False, default_flow_style = False, allow_unicode = True)

with open(filePath, "r", encoding = "utf-8") as file:
  loadConfig = yaml.safe_load(file)
  for key in saveConfig.keys():
    if key in loadConfig:
      if saveConfig[key] != loadConfig[key]:
        sys.exit("""Mismatch: {} != {}""".format(saveConfig[key], loadConfig[key]))
    else:
      sys.exit("""Undefined: {}""".format(key))
