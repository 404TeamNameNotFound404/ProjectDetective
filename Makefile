# Makefile generated by MakefileGenerator.cs
# *DO NOT EDIT*

UNREALROOTPATH = /home/bruno/Documents/Engine/C++/Linux_Unreal_Engine_5.4.4
GAMEPROJECTFILE =/home/bruno/Documents/GitHub/ProjectDetective/ProjectDetective.uproject

TARGETS = \
	DotNetPerforceLib-Linux-DebugGame  \
	DotNetPerforceLib-Linux-Shipping  \
	DotNetPerforceLib \
	EventLoopUnitTests-Linux-DebugGame  \
	EventLoopUnitTests-Linux-Shipping  \
	EventLoopUnitTests \
	UnrealEditor-Linux-DebugGame  \
	UnrealEditor-Linux-Shipping  \
	UnrealEditor \
	UnrealGame-Linux-DebugGame  \
	UnrealGame-Linux-Shipping  \
	UnrealGame \
	ProjectDetective-Linux-DebugGame  \
	ProjectDetective-Linux-Shipping  \
	ProjectDetective \
	ProjectDetectiveEditor-Linux-DebugGame  \
	ProjectDetectiveEditor-Linux-Shipping  \
	ProjectDetectiveEditor\
	configure

BUILD = bash "$(UNREALROOTPATH)/Engine/Build/BatchFiles/Linux/Build.sh"
PROJECTBUILD = "$(UNREALROOTPATH)/Engine/Binaries/ThirdParty/DotNet/6.0.302/linux/dotnet" "$(UNREALROOTPATH)/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll"

all: StandardSet

RequiredTools: CrashReportClient-Linux-Shipping CrashReportClientEditor-Linux-Shipping ShaderCompileWorker UnrealLightmass EpicWebHelper-Linux-Shipping

StandardSet: RequiredTools UnrealFrontend ProjectDetectiveEditor UnrealInsights

DebugSet: RequiredTools UnrealFrontend-Linux-Debug ProjectDetectiveEditor-Linux-Debug


DotNetPerforceLib-Linux-DebugGame:
	 $(BUILD) DotNetPerforceLib Linux DebugGame  $(ARGS)

DotNetPerforceLib-Linux-Shipping:
	 $(BUILD) DotNetPerforceLib Linux Shipping  $(ARGS)

DotNetPerforceLib:
	 $(BUILD) DotNetPerforceLib Linux Development  $(ARGS)

EventLoopUnitTests-Linux-DebugGame:
	 $(BUILD) EventLoopUnitTests Linux DebugGame  $(ARGS)

EventLoopUnitTests-Linux-Shipping:
	 $(BUILD) EventLoopUnitTests Linux Shipping  $(ARGS)

EventLoopUnitTests:
	 $(BUILD) EventLoopUnitTests Linux Development  $(ARGS)

UnrealEditor-Linux-DebugGame:
	 $(BUILD) UnrealEditor Linux DebugGame  $(ARGS)

UnrealEditor-Linux-Shipping:
	 $(BUILD) UnrealEditor Linux Shipping  $(ARGS)

UnrealEditor:
	 $(BUILD) UnrealEditor Linux Development  $(ARGS)

UnrealGame-Linux-DebugGame:
	 $(BUILD) UnrealGame Linux DebugGame  $(ARGS)

UnrealGame-Linux-Shipping:
	 $(BUILD) UnrealGame Linux Shipping  $(ARGS)

UnrealGame:
	 $(BUILD) UnrealGame Linux Development  $(ARGS)

ProjectDetective-Linux-DebugGame:
	 $(PROJECTBUILD) ProjectDetective Linux DebugGame  -project="$(GAMEPROJECTFILE)" $(ARGS)

ProjectDetective-Linux-Shipping:
	 $(PROJECTBUILD) ProjectDetective Linux Shipping  -project="$(GAMEPROJECTFILE)" $(ARGS)

ProjectDetective:
	 $(PROJECTBUILD) ProjectDetective Linux Development  -project="$(GAMEPROJECTFILE)" $(ARGS)

ProjectDetectiveEditor-Linux-DebugGame:
	 $(PROJECTBUILD) ProjectDetectiveEditor Linux DebugGame  -project="$(GAMEPROJECTFILE)" $(ARGS)

ProjectDetectiveEditor-Linux-Shipping:
	 $(PROJECTBUILD) ProjectDetectiveEditor Linux Shipping  -project="$(GAMEPROJECTFILE)" $(ARGS)

ProjectDetectiveEditor:
	 $(PROJECTBUILD) ProjectDetectiveEditor Linux Development  -project="$(GAMEPROJECTFILE)" $(ARGS)

configure:
	xbuild /property:Configuration=Development /verbosity:quiet /nologo "$(UNREALROOTPATH)/Engine/Source/Programs/UnrealBuildTool/UnrealBuildTool.csproj"
	$(PROJECTBUILD) -projectfiles -project="\"$(GAMEPROJECTFILE)\"" -game -engine 

.PHONY: $(TARGETS)
