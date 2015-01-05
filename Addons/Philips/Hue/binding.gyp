{
  "targets": [
    {
      "target_name": "hue",
      "sources": ["addon.cc" , "HueWrapper.cc" ],
      "include_dirs": [ "../../../src/",
        "<!(node -e \"require('nan')\")"
      ], 
    "link_settings": {  'library_dirs': ["../../../src"]  },
    }
  ]
}