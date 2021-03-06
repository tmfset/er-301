local app = app
local libexample = require "example.libexample"
local Class = require "Base.Class"
local Unit = require "Unit"
local GainBias = require "Unit.ViewControl.GainBias"
local InputGate = require "Unit.ViewControl.InputGate"
local Gate = require "Unit.ViewControl.Gate"
local Encoder = require "Encoder"

local AR = Class {}
AR:include(Unit)

function AR:init(args)
  args.title = "AR"
  args.mnemonic = "En"
  Unit.init(self, args)
end

function AR:onLoadGraph(channelCount)
  if channelCount == 2 then
    self:loadStereoGraph()
  else
    self:loadMonoGraph()
  end
end

function AR:loadMonoGraph()
  local gate = self:addObject("gate", app.Comparator())
  gate:setGateMode()
  local loop = self:addObject("loop", app.Comparator())
  loop:setToggleMode()
  local ar = self:addObject("ar", libexample.AR())
  local attack = self:addObject("attack", app.GainBias())
  local release = self:addObject("release", app.GainBias())
  local attackRange = self:addObject("attackRange", app.MinMax())
  local releaseRange = self:addObject("releaseRange", app.MinMax())

  connect(self, "In1", gate, "In")
  connect(gate, "Out", ar, "Gate")
  connect(loop, "Out", ar, "Loop")
  connect(ar, "Out", self, "Out1")

  connect(attack, "Out", ar, "Attack")
  connect(release, "Out", ar, "Release")

  connect(attack, "Out", attackRange, "In")
  connect(release, "Out", releaseRange, "In")

  self:addMonoBranch("loop", loop, "In", loop, "Out")
  self:addMonoBranch("attack", attack, "In", attack, "Out")
  self:addMonoBranch("release", release, "In", release, "Out")
end

function AR:loadStereoGraph()
  self:loadMonoGraph()
  connect(self.objects.ar, "Out", self, "Out2")
end

local views = {
  expanded = {
    "input",
    "loop",
    "attack",
    "release"
  },
  collapsed = {}
}

function AR:onLoadViews(objects, branches)
  local controls = {}

  controls.input = InputGate {
    button = "input",
    description = "Unit Input",
    comparator = objects.gate
  }

  controls.loop = Gate {
    button = "loop",
    description = "Loop",
    branch = branches.loop,
    comparator = objects.loop
  }

  controls.attack = GainBias {
    button = "A",
    branch = branches.attack,
    description = "Attack",
    gainbias = objects.attack,
    range = objects.attackRange,
    biasMap = Encoder.getMap("ADSR"),
    biasUnits = app.unitSecs,
    initialBias = 0.050
  }

  controls.release = GainBias {
    button = "R",
    branch = branches.release,
    description = "Release",
    gainbias = objects.release,
    range = objects.releaseRange,
    biasMap = Encoder.getMap("ADSR"),
    biasUnits = app.unitSecs,
    initialBias = 0.100
  }

  return controls, views
end

return AR
