Capture Replay
==============

To open/close capture files, use :any:`cepton_sdk.initialize` and :any:`cepton_sdk.deinitialize` methods respectively. The high level API methods will automatically resume the capture replay as necessary. 

.. autofunction:: cepton_sdk.open_replay
.. autofunction:: cepton_sdk.close_replay
.. autofunction:: cepton_sdk.capture_replay.get_filename
.. autofunction:: cepton_sdk.capture_replay.get_length
.. autofunction:: cepton_sdk.capture_replay.get_position
.. autofunction:: cepton_sdk.capture_replay.get_start_time
.. autofunction:: cepton_sdk.capture_replay.get_time
.. autofunction:: cepton_sdk.capture_replay.is_end
.. autofunction:: cepton_sdk.capture_replay.is_open
.. autofunction:: cepton_sdk.capture_replay.seek
.. autofunction:: cepton_sdk.capture_replay.seek_relative