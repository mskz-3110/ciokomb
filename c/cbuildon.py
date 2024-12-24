import sys
import os
import pathlib
sys.path.append("""{}/build""".format(os.path.dirname(__file__)))
from cbuildon_scripts import *

def move_lib(libDir, buildDirPattern, exts):
  for ext in exts:
    for path in find("""{}/*.{}""".format(buildDirPattern, ext)):
      mkdir(libDir)
      move(path, libDir)

def cmake_msvc_runtime_library(runtime, configuration):
  msvcRuntimeLibrary = "MultiThreaded"
  if configuration == "Debug":
    msvcRuntimeLibrary += configuration
  if runtime == "MD":
    msvcRuntimeLibrary += "DLL"
  return msvcRuntimeLibrary

def cmake_build(osName, libRootDir, buildConfig, isClean):
  isMoveLib = os.path.basename(os.getcwd()) == "lib"
  for generator in buildConfig.keys():
    match osName:
      case "ios":
        for combination in buildConfig[generator]:
          archs, configuration = combination.split(" ")
          buildDir = """{}""".format(configuration)
          libDir = os.path.abspath("""{}_{}""".format(libRootDir, buildDir))
          buildDir = """build/{}""".format(buildDir)
          if isClean:
            rm(buildDir)
          if os.path.isdir(buildDir) is False:
            mkdir(buildDir)
            command([
              "cmake",
              "-G", "Xcode",
              "-D", "CMAKE_MACOSX_BUNDLE=YES",
              "-D", "CMAKE_SYSTEM_NAME=iOS",
              "-D", "CMAKE_OSX_SYSROOT=iphoneos",
              "-D", """LIB_DIR={}""".format(libDir),
              "-B", buildDir,
            ])
          options = []
          for arch in archs.split(";"):
            options.append("-arch")
            options.append(arch)
          command([
              "cmake",
              "--build", buildDir,
              "--config", configuration,
              "--",
              "CODE_SIGNING_REQUIRED=NO",
              "CODE_SIGNING_ALLOWED=NO",
            ] + options)
          if isMoveLib:
            move_lib(libDir, """{}/{}-*""".format(buildDir, configuration), ["dylib", "a"])
      case "macos":
        for combination in buildConfig[generator]:
          arch, configuration = combination.split(" ")
          buildDir = """{}_{}""".format(arch, configuration)
          libDir = os.path.abspath("""{}_{}""".format(libRootDir, buildDir))
          buildDir = """build/{}""".format(buildDir)
          if isClean:
            rm(buildDir)
          if os.path.isdir(buildDir) is False:
            mkdir(buildDir)
            command([
              "cmake",
              "-G", "Xcode",
              "-D", """CMAKE_OSX_ARCHITECTURES={}""".format(arch),
              "-D", """LIB_DIR={}""".format(libDir),
              "-B", buildDir,
            ])
          command([
              "cmake",
              "--build", buildDir,
              "--config", configuration,
            ])
          if isMoveLib:
            move_lib(libDir, """{}/**""".format(buildDir), ["dylib", "a"])
      case "windows":
        for generator in buildConfig.keys():
          version = generator.split(" ")[-1]
          for combination in buildConfig[generator]:
            arch, runtime, configuration = combination.split(" ")
            buildDir = "_".join([version, arch, runtime, configuration])
            libDir = os.path.abspath("""{}/{}""".format(libRootDir, buildDir))
            buildDir = """build/{}""".format(buildDir)
            if isClean:
              rm(buildDir)
            if os.path.isdir(buildDir) is False:
              mkdir("build")
              command([
                "cmake",
                "-G", generator,
                "-A", arch,
                "-D", """CMAKE_MSVC_RUNTIME_LIBRARY={}""".format(cmake_msvc_runtime_library(runtime, configuration)),
                "-D", """LIB_DIR={}""".format(libDir),
                "-B", buildDir,
              ])
            command([
              "cmake",
              "--build", buildDir,
              "--config", configuration,
            ])
            if isMoveLib:
              move_lib(libDir, """{}/**""".format(buildDir), ["dll", "lib"])
      case "android":
        androidNdkRoot = os.environ["ANDROID_NDK_ROOT"]
        for generator in buildConfig.keys():
          for combination in buildConfig[generator]:
            platform, abi, configuration = combination.split(" ")
            buildDir = "_".join([platform, abi, configuration])
            libDir = os.path.abspath("""{}_{}""".format(libRootDir, buildDir))
            buildDir = """build/{}""".format(buildDir)
            if isClean:
              rm(buildDir)
            if os.path.isdir(buildDir) is False:
              mkdir("build")
              command([
                "cmake",
                "-G", generator,
                "-D", """CMAKE_TOOLCHAIN_FILE={}/build/cmake/android.toolchain.cmake""".format(androidNdkRoot),
                "-D", """ANDROID_PLATFORM={}""".format(platform),
                "-D", """ANDROID_ABI={}""".format(abi),
                "-D", """LIB_DIR={}""".format(libDir),
                "-B", buildDir,
              ])
            command([
              "cmake",
              "--build", buildDir,
              "--config", configuration,
            ])
            if isMoveLib:
              move_lib(libDir, """{}/**""".format(buildDir), ["so", "a"])
      case "linux":
        for configuration in buildConfig.keys():
          buildDir = "_".join([configuration])
          libDir = os.path.abspath("""{}_{}""".format(libRootDir, buildDir))
          buildDir = """build/{}""".format(buildDir)
          if isClean:
            rm(buildDir)
          if os.path.isdir(buildDir) is False:
            mkdir("build")
            command([
              "cmake",
              "-D", """LIB_DIR={}""".format(libDir),
              "-B", buildDir,
            ])
          command([
            "cmake",
            "--build", buildDir,
            "--config", configuration,
          ])
          if isMoveLib:
            move_lib(libDir, """{}/**""".format(buildDir), ["so", "a"])

def os_version(osName):
  match osName:
    case "ios":
      stdout = command(["xcrun", "--sdk", "iphoneos", "--show-sdk-version"], stdout = subprocess.PIPE).stdout
    case "macos":
      stdout = command(["sw_vers", "--productVersion"], stdout = subprocess.PIPE).stdout
    case "android":
      with open("""{}/source.properties""".format(os.environ["ANDROID_NDK_ROOT"])) as file:
        for line in file.readlines():
          key, value = line.rstrip().split(" = ")
          if key == "Pkg.ReleaseName":
            return value
      return "?"
    case "linux":
      linuxName = "?"
      version = "?"
      filePath = "/etc/os-release"
      if os.path.isfile(filePath):
        with open(filePath) as file:
          for line in file.readlines():
            key, value = line.rstrip().split("=")
            if key == "ID":
              linuxName = value
              break
      filePath = "/etc/debian_version"
      if os.path.isfile(filePath):
        version = pathlib.Path(filePath).read_text().rstrip()
      return "_".join([linuxName, version])
    case _:
      return None
  return stdout.decode("utf-8").rstrip()

def build_config_paths(osName, argv):
  if len(argv) == 0:
    return find("""build/{}/build*.yaml""".format(osName))
  cwd = getdir()
  paths = []
  for arg in argv:
    path = os.path.relpath("""{}/{}""".format(cwd, arg))
    exists_assert(path)
    paths.append(path)
  return paths

def build(osName, argv, isClean):
  oldDir = getdir()
  libRootDir = """{}/lib/{}""".format(oldDir, osName)
  osVersion = os_version(osName)
  if osVersion is not None:
    libRootDir = """{}/{}""".format(libRootDir, osVersion)
  for path in build_config_paths(osName, argv):
    with open(path, "r", encoding = "utf-8") as file:
      buildConfig = yaml.safe_load(file)
      for dirName in ["lib", "tests"]:
        buildDir = """build/{}/{}""".format(osName, dirName)
        if os.path.isdir(buildDir):
          chdir(buildDir)
          cmake_build(osName, libRootDir, buildConfig, isClean)
          chdir(oldDir)

def test_names(testNames = []):
  if len(testNames) == 0:
    return list(map(lambda filePath: os.path.splitext(os.path.basename(filePath))[0], find("tests/*.c") + find("tests/*.cpp")))
  else:
    return testNames

def test(osName, argv):
  for testName in test_names(argv):
    match osName:
      case "windows":
        pattern = """build/{}/tests/build/**/{}.exe""".format(osName, testName)
      case _:
        pattern = """build/{}/tests/build/**/{}""".format(osName, testName)
    for path in find(pattern):
      commandArgs = []
      match osName:
        case "linux":
          commandArgs = ["valgrind", "--leak-check=full", "--show-leak-kinds=all"]
      command(commandArgs + [path.replace("\\", "/")])

def setup_android():
  if "ANDROID_NDK_ROOT" not in os.environ:
    os.environ["ANDROID_NDK_ROOT"] = os.path.abspath("ext/android")

chdir(os.path.dirname(__file__))
argv = sys.argv[1:]
taskName = shift(argv, "")
match taskName:
  case "docker.build":
    command(["docker", "compose", "build"])
  case "docker.run":
    print(" ".join(["docker", "compose", "run", "--rm", "debian", "bash"]))
  case "linux.build":
    build("linux", argv, False)
  case "linux.rebuild":
    build("linux", argv, True)
  case "linux.test":
    test("linux", argv)
  case "windows.build":
    build("windows", argv, False)
  case "windows.rebuild":
    build("windows", argv, True)
  case "windows.test":
    test("windows", argv)
  case "android.build":
    setup_android()
    build("android", argv, False)
  case "android.rebuild":
    setup_android()
    build("android", argv, True)
  case "macos.build":
    build("macos", argv, False)
  case "macos.rebuild":
    build("macos", argv, True)
  case "macos.test":
    test("macos", argv)
  case "ios.build":
    build("ios", argv, False)
  case "ios.rebuild":
    build("ios", argv, True)
  case _:
    strings = []
    if 0 < len(taskName):
      strings.append("""\033[40m\033[31m{}\033[0m is undefined.""".format(taskName))
    strings.append("""<Tasks>
  docker.build

  docker.run

  linux.build

  linux.rebuild

  linux.test <test names>

  windows.build <yaml file paths>

  windows.rebuild <yaml file paths>

  windows.test <test names>

  android.build <yaml file paths>

  android.rebuild <yaml file paths>

  macos.build <yaml file paths>

  macos.rebuild <yaml file paths>

  macos.test <test names>

  ios.build <yaml file paths>

  ios.rebuild <yaml file paths>
""")
    sys.exit("\n".join(strings))
