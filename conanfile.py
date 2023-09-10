from conan import ConanFile
from conan.tools.cmake import CMake

class sound(ConanFile):
    name = "sound"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    requires = [ ("openal-soft/1.22.2")  
                ,("catch2/3.4.0")
               ]
    generators = ["CMakeToolchain" , "CMakeDeps"]
    
    def configure(self):
        self.options['vsg'].shared = False
        #this is currently waiting for the PR to be accepted
        #self.options['vsg'].shader_compiler = True
        
    def imports(self):    
        self.copy("*.dll", "bin", "bin")     
