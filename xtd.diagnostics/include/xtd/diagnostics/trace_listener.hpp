/// @file
/// @brief Contains xtd::diagnostics::debug class.
#pragma once
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <xtd/xtd.strings>
#include "trace_event_cache.hpp"
#include "trace_event_type.hpp"
#include "trace_filter.hpp"
#include "trace_options.hpp"

/// @brief The xtd namespace contains all fundamental classes to access Hardware, Os, System, and more.
namespace xtd {
  /// @brief Implements a hash table with the key and the value strongly typed to be strings rather than objects.
  /// @par Examples
  /// The following code example demonstrates several of the properties and methods of string_map.
  /// @include string_map.cpp
  using string_map = std::map<std::string, std::string>;
  
  /// @brief The xtd::diagnostics namespace provides classes that allow you to interact with system processes, event logs, and performance counters.
  namespace diagnostics {
    /// @brief Provides the abstract base class for the listeners who monitor trace and debug output.
    /// @remarks For examples of how to implement a trace_listener, see the default_trace_listener, text_writer_trace_listener and the event_log_trace_listener classes.
    /// @remarks You must enable tracing or debugging to use a trace listener. The syntax is compiler specific. If you use other than cmake to manage your build, refer to the documentation of your build nanager.
    ///  * To enable debugging with cmake, add the add_definitions(-DDEBUG) command line in the CMakeLists.txt of your porject, or you can add #define DEBUG to the top of your file but in this case, the compiler options for debug is not activated.
    ///  * To enable tracing with cmake, add the add_definitions(-DTRACE) command line in the CMakeLists.txt of your porject, or you can add #define TRACE to the top of your file.
    /// @note <b>to Inheritors:</b> Inherit from this class to implement a custom listener for the debug and trace classes. At a minimum, you must implement the write and write_line methods. Additionally, you can implement the fail, close and flush methods.
    class trace_listener {
    public:
      /// @brief Initializes a new instance of the trace_listener class.
      /// @remarks The following table shows initial property values for an instance of trace_listener.
      /// | Property     | Default Value          |
      /// |--------------|------------------------|
      /// | name         | The empty string (""). |
      /// | indent_level | Zero.                  |
      /// | indent_size  | Four spaces.           |
      trace_listener() = default;
      
      /// @cond
      trace_listener(const trace_listener& tl) = default;
      trace_listener& operator=(const trace_listener& tl) = default;
      /// @endcond
      
      /// @brief Initializes a new instance of the trace_listener class using the specified name as the listener.
      /// @param name The name of the trace_listener.
      explicit trace_listener(const std::string& name);

      /// @brief Gets the custom trace listener attributes defined in the application configuration file.
      /// @return A StringDictionary containing the custom attributes for the trace listener.
      /// @remarks Classes that inherit from the trace_listener class can add custom attributes by overriding the get_supported_attributes method and returning a string array of custom attribute names. The Attributes property identifies the custom attributes that are referenced in the application's configuration file. For example, in the following configuration file excerpt the delimited_listTrace_listener custom attribute "delimiter" is referenced. In this case, the Attributes property returns a StringDictionary containing the string "delimiter".
      const string_map& attibutes() const {return this->data_->attibutes_;}

      /// @brief Gets the custom trace listener attributes defined in the application configuration file.
      /// @return A StringDictionary containing the custom attributes for the trace listener.
      /// @remarks Classes that inherit from the trace_listener class can add custom attributes by overriding the get_supported_attributes method and returning a string array of custom attribute names. The Attributes property identifies the custom attributes that are referenced in the application's configuration file. For example, in the following configuration file excerpt the delimited_listTrace_listener custom attribute "delimiter" is referenced. In this case, the Attributes property returns a StringDictionary containing the string "delimiter".
      string_map& attibutes() {return this->data_->attibutes_;}

      /// @brief Sets the custom trace listener attributes defined in the application configuration file.
      /// @param attribute A StringDictionary containing the custom attributes for the trace listener.
      /// @remarks Classes that inherit from the trace_listener class can add custom attributes by overriding the get_supported_attributes method and returning a string array of custom attribute names. The Attributes property identifies the custom attributes that are referenced in the application's configuration file. For example, in the following configuration file excerpt the delimited_listTrace_listener custom attribute "delimiter" is referenced. In this case, the Attributes property returns a StringDictionary containing the string "delimiter".
      void attibutes(const string_map& attribute) {this->data_->attibutes_ = attribute;}

      //const trace_filter& filter() const {*return this->data_->filter;}
      //void filter(const trace_filter& filer) {this->data_->filter = &filter;}
      
      /// @brief Gets the indent level.
      /// @return unsigned int The indent level. The default is zero.
      /// @remarks The IndentLevel property represents the number of times that the indent specified by the IndentSize property is applied. This property is stored on per-thread/per-request basis.
      unsigned int indent_level() const {return this->data_->indent_level_;}

      /// @brief Sets the indent level.
      /// @param indent_level The indent level. The default is zero.
      /// @remarks The IndentLevel property represents the number of times that the indent specified by the IndentSize property is applied. This property is stored on per-thread/per-request basis.
      void indent_level(unsigned int indent_level) {this->data_->indent_level_ = indent_level;}
      
      /// @brief Gets the number of spaces in an indent.
      /// @return unsigned int The number of spaces in an indent. The default is four spaces.
      /// @remarks The property is stored on per-thread/per-request basis.
      unsigned int indent_size() const {return this->data_->indent_size_;}

      /// @brief Sets the number of spaces in an indent.
      /// @param int32 The number of spaces in an indent. The default is four spaces.
      /// @remarks The property is stored on per-thread/per-request basis.
      void indent_size(unsigned int indent_size) {this->data_->indent_size_ = indent_size;}

      /// @brief Gets a value indicating whether the trace listener is thread safe.
      /// @return bool true if the trace listener is thread safe; otherwise, false. The default is false.
      /// @remarks The value of IsThreadSafe is used to determine whether to use a global lock when writing to the listener. If the value of IsThreadSafe is false, the global lock is used regardless of the value of UseGlobalLock. The global lock is not used only if the value of IsThreadSafe is true and the value of UseGlobalLock is false. The default behavior is to use the global lock whenever writing to the listener.
      virtual bool is_thread_safe() const {return this->data_->is_thread_safe_;}

      /// @brief Gets or sets a name for this TraceListener.
      /// @return string A name for this TraceListener. The default is an empty string ("").
      /// @remarks The name can be used to organize and access listeners in a TraceListenerCollection collection.
      const std::string& name() const {return this->data_->name_;}

      /// @brief Sets a name for this TraceListener.
      /// @param name A name for this TraceListener. The default is an empty string ("").
      /// @remarks The name can be used to organize and access listeners in a TraceListenerCollection collection.
      void name(const std::string& name) {this->data_->name_ = name;}
      
      /// @brief Gets the trace output options.
      /// @return trace_options A bitwise combination of the enumeration values. The default is None.
      /// @remarks The TraceOutputOptions property determines the optional content of trace output. The property can be set in the configuration file or programmatically during execution to include additional data specifically for a section of code. For example, you can set the TraceOutputOptions property for the console trace listener to TraceOptions.Callstack to add call stack information to the trace output.
      /// @remarks The TraceOptions enumeration is not used by the following classes and methods:
      /// * The EventLogTraceListener class, because it can cause a large volume of data to be written to the log.
      /// * The Write and WriteLine methods of the ConsoleTraceListener, DefaultTraceListener, and TextWriterTraceListener classes.
      /// * The Write and WriteLine methods of the TraceListener class when they are not overridden in a derived class.
      const trace_options& trace_output_options() const {return this->data_->trace_output_options_;}

      /// @brief Sets the trace output options.
      /// @param trace_output_options A bitwise combination of the enumeration values. The default is None.
      /// @remarks The TraceOutputOptions property determines the optional content of trace output. The property can be set in the configuration file or programmatically during execution to include additional data specifically for a section of code. For example, you can set the TraceOutputOptions property for the console trace listener to TraceOptions.Callstack to add call stack information to the trace output.
      /// @remarks The TraceOptions enumeration is not used by the following classes and methods:
      /// * The EventLogTraceListener class, because it can cause a large volume of data to be written to the log.
      /// * The Write and WriteLine methods of the ConsoleTraceListener, DefaultTraceListener, and TextWriterTraceListener classes.
      /// * The Write and WriteLine methods of the TraceListener class when they are not overridden in a derived class.
      void trace_output_options(const trace_options& trace_output_options) {this->data_->trace_output_options_ = trace_output_options;}

      /// @brief When overridden in a derived class, closes the output stream so it no longer receives tracing or debugging output.
      /// @remarks Use this method when the output is going to a file, such as to the TextWriterTraceListener. After a call to this method, you must reinitialize the object.
      virtual void close();
      
      /// @brief Emits an error message to the listener you create when you implement the TraceListener class.
      /// @param message A message to emit.
      /// @remarks The default behavior is to display the specified message in a message box when the application is running in a user-interface mode, and to the TraceListener instances in a TraceListenerCollection collection. By default, the TraceListenerCollection collection has an instance of a DefaultTraceListener. You can customize this behavior by adding a TraceListener to or removing one from the collection.
      virtual void fail(const std::string& message);
      
      /// @brief Emits the specified error message.
      /// @param message A message to emit.
      /// @param detailMessage A detailed message to emit.
      /// @remarks The default behavior is to display the message and detailed message in a message box when the application is running in a user-interface mode, and to the TraceListener instances in a TraceListenerCollection collection. By default, the TraceListenerCollection collection has an instance of a DefaultTraceListener. You can customize this behavior by adding a TraceListener to or removing one from the collection.
      virtual void fail(const std::string& message, const std::string& detailMessage);
      
      /// @brief When overridden in a derived class, flushes the output buffer.
      virtual void flush();
      
      /// @brief Writes trace information, a data object and event information to the listener specific output.
      /// @param eventCache A TraceEventCache object that contains the current process ID, thread ID, and stack trace information.
      /// @param source A name used to identify the output, typically the name of the application that generated the trace event.
      /// @param eventType One of the TraceEventType values specifying the type of event that has caused the trace.
      /// @param id A numeric identifier for the event.
      /// @param data The trace data to emit.
      /// @remarks <b>Important</b> This method is not intended to be called directly by application code but by members of the Debug, Trace, and TraceSource classes to write trace data to output.
      /// @remarks The default implementation writes the eventCache, source, eventType and id parameters in the header and footer of the trace. The data parameter is written as the body of the trace message. The ToString method of the data object is used to convert the object to a String.
      template<typename object>
      void trace_data(const xtd::diagnostics::trace_event_cache& event_cache, const std::string& source, const xtd::diagnostics::trace_event_type& event_type, int id, const object& data) {
        this->write_line(xtd::strings::format("{} {}: {} : {}", source, event_type, id, data));
        this->write_event_cache(event_cache);
      }
      
      /// @brief Writes trace information, a data object and event information to the listener specific output.
      /// @param eventCache A TraceEventCache object that contains the current process ID, thread ID, and stack trace information.
      /// @param source A name used to identify the output, typically the name of the application that generated the trace event.
      /// @param eventType One of the TraceEventType values specifying the type of event that has caused the trace.
      /// @param id A numeric identifier for the event.
      /// @param data The trace data to emit.
      /// @remarks <b>Important</b> This method is not intended to be called directly by application code but by members of the Debug, Trace, and TraceSource classes to write trace data to output.
      /// @remarks The default implementation writes the eventCache, source, eventType and id parameters in the header and footer of the trace. The data parameter is written as the body of the trace message. The ToString method of the data object is used to convert the object to a String.
      template<typename object>
      void trace_data(const xtd::diagnostics::trace_event_cache& event_cache, const std::string& source, const xtd::diagnostics::trace_event_type& event_type, int id, const std::vector<object>& data) {
        this->write_line(xtd::strings::format("{} {}: {} : {}", source, event_type, id, data));
        this->write_event_cache(event_cache);
      }
      
      /// @brief Writes trace information, an array of data objects and event information to the listener specific output.
      /// @param eventCache A TraceEventCache object that contains the current process ID, thread ID, and stack trace information.
      /// @param source A name used to identify the output, typically the name of the application that generated the trace event.
      /// @param eventType One of the TraceEventType values specifying the type of event that has caused the trace.
      /// @param id A numeric identifier for the event.
      /// @param data An array of objects to emit as data.
      /// @remarks <b>Important</b> This method is not intended to be called directly by application code but by members of the Debug, Trace, and TraceSource classes to write trace data to output.
      /// @remarks The default implementation writes the eventCache, source, eventType and id parameters in the header and footer of the trace. The data parameter is written as the body of the trace message. The ToString method of the data object is used to convert the object to a String.
      template<typename ...objects>
      void trace_data(const xtd::diagnostics::trace_event_cache& event_cache, const std::string& source, const xtd::diagnostics::trace_event_type& event_type, int id, const objects& ... data) {
        this->write_line(xtd::strings::format("{} {}: {} : {}", source, event_type, id, xtd::strings::join(", ", {data...})));
        this->write_event_cache(event_cache);
      }
      
      /// @brief Writes trace and event information to the listener specific output.
      /// @param eventCache A TraceEventCache object that contains the current process ID, thread ID, and stack trace information.
      /// @param source A name used to identify the output, typically the name of the application that generated the trace event.
      /// @param eventType One of the TraceEventType values specifying the type of event that has caused the trace.
      /// @param id A numeric identifier for the event.
      /// @remarks <b>Important</b> This method is not intended to be called directly by application code but by members of the Debug, Trace, and TraceSource classes to write trace data to output.
      /// @remarks The default implementation writes the values of the source, eventType and id parameters as a header. The eventCache data is written as a footer, the nature of the output data being dependent on the value of the TraceOutputOptions property.
      virtual void trace_event(const xtd::diagnostics::trace_event_cache& event_cache, const std::string& source, const xtd::diagnostics::trace_event_type& event_type, int id);
      
      /// @brief Writes trace information, a message, and event information to the listener specific output.
      /// @param eventCache A TraceEventCache object that contains the current process ID, thread ID, and stack trace information.
      /// @param source A name used to identify the output, typically the name of the application that generated the trace event.
      /// @param eventType One of the TraceEventType values specifying the type of event that has caused the trace.
      /// @param id A numeric identifier for the event.
      /// @param message A message to write.
      /// @remarks <b>Important</b> This method is not intended to be called directly by application code but by members of the Debug, Trace, and TraceSource classes to write trace data to output.
      /// @remarks The default implementation writes the values of the source, eventType and id parameters as a header. The eventCache data is written as a footer, the nature of the output data being dependent on the value of the TraceOutputOptions property.
      virtual void trace_event(const xtd::diagnostics::trace_event_cache& eventCache, const std::string& source, const xtd::diagnostics::trace_event_type& event_type, int id, const std::string& message);
      
      /// @brief Writes trace information, a formatted array of objects and event information to the listener specific output.
      /// @param eventCache A TraceEventCache object that contains the current process ID, thread ID, and stack trace information.
      /// @param source A name used to identify the output, typically the name of the application that generated the trace event.
      /// @param eventType One of the TraceEventType values specifying the type of event that has caused the trace.
      /// @param id A numeric identifier for the event.
      /// @param format A format string that contains zero or more format items, which correspond to objects in the args array.
      /// @param args An object array containing zero or more objects to format.
      /// @remarks <b>Important</b> This method is not intended to be called directly by application code but by members of the Debug, Trace, and TraceSource classes to write trace data to output.
      /// @remarks The default implementation writes the values of the source, eventType and id parameters as a header. The args object array is converted to a string using the Format method, passing the format string and args array to format the string as the message portion of the trace. The eventCache data is written as a footer, the nature of the output data being dependent on the value of the TraceOutputOptions property.
      template<typename ...objects>
      void trace_event(const xtd::diagnostics::trace_event_cache& event_cache, const std::string& source, const xtd::diagnostics::trace_event_type& event_type, int id, const std::string& format, const objects& ... args) {
        this->write_line(xtd::strings::format("{} {}: {} : {}", source, event_type, id, xtd::strings::format(format, args...)));
        this->write_event_cache(event_cache);
      }
      
      /// @brief Writes trace information, a message, a related activity identity and event information to the listener specific output.
      /// @param eventCache A TraceEventCache object that contains the current process ID, thread ID, and stack trace information.
      /// @param source A name used to identify the output, typically the name of the application that generated the trace event.
      /// @param id A numeric identifier for the event.
      /// @param message A message to write.
      /// @param relatedActivityId A Guid object identifying a related activity.
      /// @remarks <b>Important</b> This method is not intended to be called directly by application code but by members of the Debug, Trace, and TraceSource classes to write trace data to output.
      /// @remarks The TraceTransfer method is used for the correlation of related traces. The TraceTransfer method calls the TraceEvent method to process the call, with the eventType level set to Transfer and the relatedActivityIdGuid as a string appended to the message.
      virtual void trace_transfert(const xtd::diagnostics::trace_event_cache& event_cache, const std::string& source, int id, const std::string& message, const xtd::guid& related_activity_id);
      
      /// @brief Writes the value of the object's ToString method to the listener you create when you implement the TraceListener class.
      /// @param o An Object whose fully qualified class name you want to write.
      template <typename object>
      void write(const object& o) {
        write(xtd::strings::format("{}", o));
      }
      
      /// @brief Writes a category name and the value of the object's ToString method to the listener you create when you implement the TraceListener class.
      /// @param o An Object whose fully qualified class name you want to write.
      /// @param category A category name used to organize the output.
      template <typename object>
      void write(const object& o, const std::string& category) {
        write(xtd::strings::format("{} : {}", o), category);
      }

      virtual void write(const std::string& message) = 0;
      
      /// @brief Writes the value of the object's ToString method to the listener you create when you implement the TraceListener class.
      /// @param o An Object whose fully qualified class name you want to write.
      template <typename object>
      void write_line(const object& o) {
        write_line(xtd::strings::format("{}", o));
      }
      
      /// @brief Writes a category name and the value of the object's ToString method to the listener you create when you implement the TraceListener class.
      /// @param o An Object whose fully qualified class name you want to write.
      /// @param category A category name used to organize the output.
      template <typename object>
      void write_line(const object& o, const std::string& category) {
        write_line(xtd::strings::format("{} : {}", o), category);
      }

      virtual void write_line(const std::string& message) = 0;
      
    protected:
      bool need_indent() const {return this->data_->need_indent_;}
      void need_indent(bool need_indent) const {this->data_->need_indent_ = need_indent;}

      /// @brief Writes the indent to the listener you create when you implement this class, and resets the NeedIndent property to false.
      /// @remarks This method writes the indent and resets the NeedIndent property to false. Call this method if NeedIndent is true when you are overriding the Write and WriteLine methods. By default, this method uses blank spaces for indentation. The size of the indent is determined by the values of the IndentSize and IndentLevel properties. The IndentLevel property represents the number of times the indent of the size specified by the IndentSize property is applied. This method is called by the DefaultTraceListener and TextWriterTraceListener classes.
      virtual void write_indent();

    private:
      void write_event_cache(const trace_event_cache& event_cache);
      
      struct data {
        string_map attibutes_;
        const trace_filter* filter_;
        unsigned int indent_level_ = 0;
        unsigned int indent_size_ = 4;
        bool is_thread_safe_ = false;
        std::string name_;
        bool need_indent_ = true;
        trace_options trace_output_options_ = trace_options::none;
      };
      
      std::shared_ptr<data> data_ = std::make_shared<data>();
    };
  }
}