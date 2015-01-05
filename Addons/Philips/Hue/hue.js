var addon = require('bindings')('hue');

var obj = new addon.HueWrapper();
console.log(obj.connect());
